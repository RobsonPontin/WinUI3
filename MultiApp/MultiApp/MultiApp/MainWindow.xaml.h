#pragma once

#include "MainWindow.g.h"
#include "..\ProcessBridge\ProcessBridge.h"

#include <memory>
#include <thread>
#include <atomic>

namespace winrt::MultiApp::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow()
            : m_launcher(::ProcessBridge::CreateChildProcessLauncher({ .childExeName = L"ChildApp.exe" })),
              m_ipcClient(::ProcessBridge::Ipc::CreateNamedPipeClient())
        {
        }

        // Constructor accepting custom dependencies (for testing / DI).
        MainWindow(std::shared_ptr<::ProcessBridge::IChildProcessLauncher> launcher,
                   std::shared_ptr<::ProcessBridge::Ipc::IIpcClient> ipcClient)
            : m_launcher(std::move(launcher)),
              m_ipcClient(std::move(ipcClient))
        {
        }

        ~MainWindow();

        void LaunchChildButton_Click(
            winrt::Windows::Foundation::IInspectable const& sender,
            winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args);

    private:
        enum class IpcColor { Green, Yellow, Red };

        void LaunchAndConnectAsync();
        void UpdateIpcStatus(const std::wstring& status, IpcColor color);
        void ReEnableLaunchButton();
        void StopIpcMonitor();

        std::shared_ptr<::ProcessBridge::IChildProcessLauncher> m_launcher;
        std::shared_ptr<::ProcessBridge::Ipc::IIpcClient> m_ipcClient;
        winrt::Microsoft::UI::Dispatching::DispatcherQueue m_dispatcherQueue{ nullptr };

        std::thread m_ipcThread;
        std::atomic<bool> m_ipcRunning{ false };
        std::atomic<bool> m_closing{ false };
    };
}

namespace winrt::MultiApp::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
