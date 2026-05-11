#pragma once

#include "IIpcClient.h"

#include <windows.h>

namespace ProcessBridge::Ipc
{
    class NamedPipeClient : public IIpcClient
    {
    public:
        NamedPipeClient() = default;
        ~NamedPipeClient() override;

        NamedPipeClient(const NamedPipeClient&) = delete;
        NamedPipeClient& operator=(const NamedPipeClient&) = delete;

        bool Connect(const std::wstring& pipeName,
                     DWORD timeoutMs = 5000) override;
        void Disconnect() override;
        bool IsConnected() const override;

        std::optional<IpcMessage> Send(const IpcMessage& request,
                                       DWORD timeoutMs = 5000) override;

    private:
        HANDLE m_pipe{ INVALID_HANDLE_VALUE };
    };
}
