#pragma once

#include "ChildAppMainWindow.g.h"
#include "..\ProcessBridge\ProcessBridge.h"

#include <memory>
#include <atomic>
#include <mutex>
#include <string>

namespace winrt::ChildApp::implementation
{
    struct ChildAppMainWindow : ChildAppMainWindowT<ChildAppMainWindow>
    {
        ChildAppMainWindow();
        ~ChildAppMainWindow();

        ChildAppMainWindow(const ChildAppMainWindow&) = delete;
        ChildAppMainWindow& operator=(const ChildAppMainWindow&) = delete;

        void SendRandomButton_Click(
            winrt::Windows::Foundation::IInspectable const& sender,
            winrt::Microsoft::UI::Xaml::RoutedEventArgs const& args);

    private:
        enum class IpcColor { Green, Yellow, Red };

        void StartIpcServer();
        void UpdateIpcStatus(const std::wstring& status, IpcColor color);

        std::shared_ptr<::ProcessBridge::Ipc::IIpcServer> m_ipcServer;
        std::wstring m_pipeName;
        winrt::Microsoft::UI::Dispatching::DispatcherQueue m_dispatcherQueue{ nullptr };
        std::atomic<bool> m_closing{ false };

        // Pending random number to deliver to the parent on the next request.
        std::mutex m_pendingMutex;
        std::wstring m_pendingPayload;
    };
}

namespace winrt::ChildApp::factory_implementation
{
    struct ChildAppMainWindow : ChildAppMainWindowT<ChildAppMainWindow, implementation::ChildAppMainWindow>
    {
    };
}
