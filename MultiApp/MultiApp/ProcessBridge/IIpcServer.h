#pragma once

#include "ProcessBridgeApi.h"
#include "IpcMessage.h"

#include <functional>
#include <memory>

namespace ProcessBridge::Ipc
{
    // Callback invoked when a message is received from the client.
    // The handler populates the response which is sent back.
    using IpcRequestHandler = std::function<void(const IpcMessage& request,
                                                 IpcMessage& response)>;

    // Server-side IPC interface (used by the child app).
    // Creates a named pipe and listens for incoming client connections.
    struct IIpcServer
    {
        virtual ~IIpcServer() = default;

        // Start listening on the given pipe name.
        // The handler is called on the listener thread for each request.
        virtual bool Start(const std::wstring& pipeName,
                           IpcRequestHandler handler) = 0;

        virtual void Stop() = 0;
        virtual bool IsRunning() const = 0;
    };

    // Factory for the default named-pipe server implementation.
    PROCESSBRIDGE_API std::shared_ptr<IIpcServer> CreateNamedPipeServer();
}
