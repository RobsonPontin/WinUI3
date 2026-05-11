#include "pch.h"
#include "NamedPipeServer.h"

namespace ProcessBridge::Ipc
{
    NamedPipeServer::~NamedPipeServer()
    {
        Stop();
    }

    bool NamedPipeServer::Start(const std::wstring& pipeName,
                                IpcRequestHandler handler)
    {
        if (m_running.load())
            return false;

        m_pipeName = pipeName;
        m_handler = std::move(handler);

        m_stopEvent = ::CreateEventW(nullptr, TRUE, FALSE, nullptr);
        if (!m_stopEvent)
            return false;

        m_running.store(true);
        m_listenerThread = std::thread(&NamedPipeServer::ListenerLoop, this);
        return true;
    }

    void NamedPipeServer::Stop()
    {
        if (!m_running.load())
            return;

        m_running.store(false);

        if (m_stopEvent)
        {
            ::SetEvent(m_stopEvent);
        }

        // Unblock ConnectNamedPipe by connecting and immediately closing.
        HANDLE dummy = ::CreateFileW(
            m_pipeName.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            0, nullptr,
            OPEN_EXISTING, 0, nullptr);
        if (dummy != INVALID_HANDLE_VALUE)
        {
            ::CloseHandle(dummy);
        }

        if (m_listenerThread.joinable())
        {
            m_listenerThread.join();
        }

        if (m_stopEvent)
        {
            ::CloseHandle(m_stopEvent);
            m_stopEvent = nullptr;
        }
    }

    bool NamedPipeServer::IsRunning() const
    {
        return m_running.load();
    }

    void NamedPipeServer::ListenerLoop()
    {
        while (m_running.load())
        {
            // Create a new pipe instance for each connection cycle.
            HANDLE pipe = ::CreateNamedPipeW(
                m_pipeName.c_str(),
                PIPE_ACCESS_DUPLEX,
                PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
                PIPE_UNLIMITED_INSTANCES,
                4096, 4096,
                0, nullptr);

            if (pipe == INVALID_HANDLE_VALUE)
            {
                break;
            }

            // Wait for a client to connect.
            BOOL connected = ::ConnectNamedPipe(pipe, nullptr)
                             ? TRUE
                             : (::GetLastError() == ERROR_PIPE_CONNECTED);

            if (!connected || !m_running.load())
            {
                ::CloseHandle(pipe);
                continue;
            }

            // Service this connection: read requests and send responses
            // until the client disconnects or the server is stopped.
            while (m_running.load())
            {
                IpcMessage request;
                if (!Wire::Read(pipe, request))
                    break;

                IpcMessage response;
                if (m_handler)
                {
                    m_handler(request, response);
                }

                if (!Wire::Write(pipe, response))
                    break;

                ::FlushFileBuffers(pipe);
            }

            ::DisconnectNamedPipe(pipe);
            ::CloseHandle(pipe);
        }
    }

    std::shared_ptr<IIpcServer> CreateNamedPipeServer()
    {
        return std::make_shared<NamedPipeServer>();
    }
}
