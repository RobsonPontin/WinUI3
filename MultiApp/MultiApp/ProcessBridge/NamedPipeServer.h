#pragma once

#include "IIpcServer.h"

#include <atomic>
#include <thread>
#include <windows.h>

namespace ProcessBridge::Ipc
{
    class NamedPipeServer : public IIpcServer
    {
    public:
        NamedPipeServer() = default;
        ~NamedPipeServer() override;

        NamedPipeServer(const NamedPipeServer&) = delete;
        NamedPipeServer& operator=(const NamedPipeServer&) = delete;

        bool Start(const std::wstring& pipeName,
                   IpcRequestHandler handler) override;
        void Stop() override;
        bool IsRunning() const override;

    private:
        void ListenerLoop();

        IpcRequestHandler m_handler;
        std::wstring m_pipeName;
        std::atomic<bool> m_running{ false };
        HANDLE m_stopEvent{ nullptr };
        std::thread m_listenerThread;
    };
}
