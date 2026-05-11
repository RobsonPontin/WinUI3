#pragma once

#include "ProcessBridgeApi.h"
#include "IpcMessage.h"

#include <memory>
#include <optional>

namespace ProcessBridge::Ipc
{
    // Client-side IPC interface (used by the parent app).
    // Connects to a named pipe server and sends request/response messages.
    struct IIpcClient
    {
        virtual ~IIpcClient() = default;

        virtual bool Connect(const std::wstring& pipeName,
                             DWORD timeoutMs = 5000) = 0;
        virtual void Disconnect() = 0;
        virtual bool IsConnected() const = 0;

        // Send a request and wait for the response.
        // Returns std::nullopt on communication failure.
        virtual std::optional<IpcMessage> Send(const IpcMessage& request,
                                               DWORD timeoutMs = 5000) = 0;
    };

    // Factory for the default named-pipe client implementation.
    PROCESSBRIDGE_API std::shared_ptr<IIpcClient> CreateNamedPipeClient();
}
