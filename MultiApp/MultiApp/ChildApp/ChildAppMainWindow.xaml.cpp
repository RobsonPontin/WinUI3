#include "pch.h"
#include "ChildAppMainWindow.xaml.h"
#if __has_include("ChildAppMainWindow.g.cpp")
#include "ChildAppMainWindow.g.cpp"
#endif

#include <shellapi.h>
#include <random>

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::ChildApp::implementation
{
    ChildAppMainWindow::ChildAppMainWindow()
    {
        // Capture the DispatcherQueue before any background work starts.
        m_dispatcherQueue = winrt::Microsoft::UI::Dispatching::DispatcherQueue::GetForCurrentThread();

        // Parse the pipe name from command-line args.
        int argc = 0;
        LPWSTR* argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);
        if (argv)
        {
            ::ProcessBridge::Protocol::TryGetSwitchValue(
                argc, argv, ::ProcessBridge::Protocol::kSwitchPipeName, m_pipeName);
            ::LocalFree(argv);
        }

        if (!m_pipeName.empty())
        {
            StartIpcServer();
        }
    }

    ChildAppMainWindow::~ChildAppMainWindow()
    {
        // Signal closure before teardown to prevent queued UI updates
        // from accessing destroyed XAML controls.
        m_closing.store(true);

        if (m_ipcServer)
        {
            m_ipcServer->Stop();
        }
    }

    void ChildAppMainWindow::StartIpcServer()
    {
        m_ipcServer = ::ProcessBridge::Ipc::CreateNamedPipeServer();

        // Handler runs on the server's listener thread — not the UI thread.
        // The server is stopped in the destructor before members are torn down,
        // so `this` is valid whenever the handler executes.
        auto handler = [this](const ::ProcessBridge::Ipc::IpcMessage& request,
                              ::ProcessBridge::Ipc::IpcMessage& response)
        {
            UpdateIpcStatus(L"IPC Server: Client connected", IpcColor::Green);

            if (request.command == L"status")
            {
                response.command = L"status";
                response.payload = L"ready";
            }
            else if (request.command == L"ping")
            {
                // If there's a pending payload (e.g. a random number),
                // deliver it to the parent with this response.
                std::lock_guard lock{ m_pendingMutex };
                if (!m_pendingPayload.empty())
                {
                    response.command = L"random";
                    response.payload = std::move(m_pendingPayload);
                    m_pendingPayload.clear();
                }
                else
                {
                    response.command = L"pong";
                }
            }
            else
            {
                response.command = L"error";
                response.payload = L"unknown command";
            }
        };

        if (m_ipcServer->Start(m_pipeName, handler))
        {
            UpdateIpcStatus(L"IPC Server: Listening on pipe", IpcColor::Yellow);
        }
        else
        {
            UpdateIpcStatus(L"IPC Server: Failed to start", IpcColor::Red);
        }
    }

    void ChildAppMainWindow::SendRandomButton_Click(
        winrt::Windows::Foundation::IInspectable const& /*sender*/,
        winrt::Microsoft::UI::Xaml::RoutedEventArgs const& /*args*/)
    {
        static std::mt19937 rng{ std::random_device{}() };
        std::uniform_int_distribution<int> dist(1, 1000);
        int number = dist(rng);

        // Store for delivery on the next parent ping.
        {
            std::lock_guard lock{ m_pendingMutex };
            m_pendingPayload = std::to_wstring(number);
        }

        RandomNumberText().Text(L"Sent: " + winrt::to_hstring(number));
    }

    void ChildAppMainWindow::UpdateIpcStatus(const std::wstring& status, IpcColor color)
    {
        if (!m_dispatcherQueue || m_closing.load())
            return;

        // Marshal UI update to the dispatcher thread; safe to capture `this`
        // because the server is stopped in the destructor (joining the listener
        // thread) before members are destroyed. The m_closing check guards
        // against post-teardown XAML access.
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
}
