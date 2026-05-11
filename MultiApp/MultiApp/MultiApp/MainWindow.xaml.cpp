#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::MultiApp::implementation
{
    MainWindow::~MainWindow()
    {
        // Signal closure before teardown to prevent queued UI updates
        // from accessing destroyed XAML controls.
        m_closing.store(true);
        StopIpcMonitor();
    }

    void MainWindow::LaunchChildButton_Click(
        winrt::Windows::Foundation::IInspectable const& /*sender*/,
        winrt::Microsoft::UI::Xaml::RoutedEventArgs const& /*args*/)
    {
        // DispatcherQueue must be captured on the UI thread; background
        // threads use it to marshal status updates back to the UI.
        if (!m_dispatcherQueue)
        {
            m_dispatcherQueue = winrt::Microsoft::UI::Dispatching::DispatcherQueue::GetForCurrentThread();
        }

        // Clean up any previous IPC session before starting a new one.
        StopIpcMonitor();

        // Disable the button while launch is in progress.
        LaunchChildButton().IsEnabled(false);
        StatusText().Text(L"Launching child process...");

        LaunchAndConnectAsync();
    }

    void MainWindow::LaunchAndConnectAsync()
    {
        m_ipcRunning.store(true);

        // Capture a strong reference to keep the WinRT object alive for the
        // entire duration of the worker thread. Raw `this` would dangle if
        // the XAML framework released its last reference while the thread runs.
        auto strong = get_strong();

        // Launch + IPC connect run entirely off the UI thread so the
        // window stays responsive during process creation and handshake.
        m_ipcThread = std::thread([strong, this]()
        {
            auto result = m_launcher->Launch();

            // Marshal launch result back to the UI thread.
            // Safe to capture `this`: the strong ref keeps the object alive,
            // and m_closing guards against post-teardown XAML access.
            m_dispatcherQueue.TryEnqueue([this,
                                          msg = winrt::hstring{ result.message },
                                          ok = result.succeeded]()
            {
                if (m_closing.load())
                    return;

                StatusText().Text(msg);
                if (!ok)
                {
                    LaunchChildButton().IsEnabled(true);
                }
            });

            if (!result.succeeded || result.pipeName.empty())
            {
                m_ipcRunning.store(false);
                ReEnableLaunchButton();
                return;
            }

            // --- IPC connect & monitor (same thread) ---

            UpdateIpcStatus(L"IPC: Connecting...", IpcColor::Yellow);

            // The child process needs time to start its pipe server;
            // Connect() retries internally until the pipe appears or
            // the timeout (5 s) expires.
            if (!m_ipcClient->Connect(result.pipeName, 5000))
            {
                UpdateIpcStatus(L"IPC: Connection failed", IpcColor::Red);
                m_ipcRunning.store(false);
                ReEnableLaunchButton();
                return;
            }

            UpdateIpcStatus(L"IPC: Connected", IpcColor::Green);

            // Initial status query confirms the channel is functional.
            auto response = m_ipcClient->Send({ L"status", L"" });
            if (response)
            {
                UpdateIpcStatus(L"IPC: Connected — child status: " + response->payload, IpcColor::Green);
            }
            else
            {
                UpdateIpcStatus(L"IPC: Connected but status query failed", IpcColor::Yellow);
            }

            // Health-check loop: ping every 2 s to detect child exit.
            // Also picks up any data the child wants to push (e.g. random numbers).
            while (m_ipcRunning.load() && m_ipcClient->IsConnected())
            {
                ::Sleep(2000);

                if (!m_ipcRunning.load())
                    break;

                auto ping = m_ipcClient->Send({ L"ping", L"" });
                if (!ping)
                {
                    UpdateIpcStatus(L"IPC: Disconnected (child exited)", IpcColor::Red);
                    break;
                }

                // If the child sent data, display it on the UI thread.
                if (ping->command == L"random" && !ping->payload.empty())
                {
                    winrt::hstring val{ ping->payload };
                    m_dispatcherQueue.TryEnqueue([this, val]()
                    {
                        if (m_closing.load())
                            return;
                        ReceivedDataText().Text(L"Received from child: " + val);
                    });
                }
            }

            m_ipcClient->Disconnect();
            m_ipcRunning.store(false);

            // Re-enable the launch button so the user can spawn another child.
            ReEnableLaunchButton();
        });
    }

    void MainWindow::ReEnableLaunchButton()
    {
        if (!m_dispatcherQueue || m_closing.load())
            return;

        m_dispatcherQueue.TryEnqueue([this]()
        {
            if (m_closing.load())
                return;
            LaunchChildButton().IsEnabled(true);
        });
    }

    void MainWindow::UpdateIpcStatus(const std::wstring& status, IpcColor color)
    {
        if (!m_dispatcherQueue || m_closing.load())
            return;

        // Marshal UI update to the dispatcher thread; safe to capture `this`
        // because the worker thread holds a strong ref that keeps the object
        // alive. The m_closing check guards against post-teardown XAML access.
        winrt::hstring text{ status };
        m_dispatcherQueue.TryEnqueue([this, text, color]()
        {
            if (m_closing.load())
                return;

            IpcStatusText().Text(text);

            winrt::Microsoft::UI::Xaml::Media::SolidColorBrush brush{ winrt::Windows::UI::Color{} };
            switch (color)
            {
            case IpcColor::Green:
                brush = winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{
                    winrt::Windows::UI::Color{ 255, 0, 180, 0 } };
                break;
            case IpcColor::Yellow:
                brush = winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{
                    winrt::Windows::UI::Color{ 255, 200, 170, 0 } };
                break;
            case IpcColor::Red:
                brush = winrt::Microsoft::UI::Xaml::Media::SolidColorBrush{
                    winrt::Windows::UI::Color{ 255, 220, 30, 30 } };
                break;
            }
            IpcStatusText().Foreground(brush);
        });
    }

    void MainWindow::StopIpcMonitor()
    {
        m_ipcRunning.store(false);

        // Close the pipe handle first to unblock any in-progress
        // Read/Write call on the IPC thread.
        if (m_ipcClient)
        {
            m_ipcClient->Disconnect();
        }

        if (m_ipcThread.joinable())
        {
            m_ipcThread.join();
        }
    }
}
