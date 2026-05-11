#include "pch.h"
#include "NamedPipeClient.h"

namespace ProcessBridge::Ipc
{
    NamedPipeClient::~NamedPipeClient()
    {
        Disconnect();
    }

    bool NamedPipeClient::Connect(const std::wstring& pipeName, DWORD timeoutMs)
    {
        Disconnect();

        // WaitNamedPipeW fails immediately with ERROR_FILE_NOT_FOUND if the
        // pipe hasn't been created yet. Retry with back-off until the overall
        // timeout expires so the child process has time to start its server.
        constexpr DWORD kRetryIntervalMs = 200;
        DWORD elapsed = 0;

        while (elapsed < timeoutMs)
        {
            if (::WaitNamedPipeW(pipeName.c_str(), timeoutMs - elapsed))
                break; // Pipe is available.

            if (::GetLastError() != ERROR_FILE_NOT_FOUND)
                return false; // Unexpected error — don't retry.

            ::Sleep(kRetryIntervalMs);
            elapsed += kRetryIntervalMs;
        }

        if (elapsed >= timeoutMs)
            return false;

        m_pipe = ::CreateFileW(
            pipeName.c_str(),
            GENERIC_READ | GENERIC_WRITE,
            0, nullptr,
            OPEN_EXISTING,
            0, nullptr);

        if (m_pipe == INVALID_HANDLE_VALUE)
        {
            return false;
        }

        // Switch to byte-mode reading.
        DWORD mode = PIPE_READMODE_BYTE;
        ::SetNamedPipeHandleState(m_pipe, &mode, nullptr, nullptr);

        return true;
    }

    void NamedPipeClient::Disconnect()
    {
        if (m_pipe != INVALID_HANDLE_VALUE)
        {
            ::CloseHandle(m_pipe);
            m_pipe = INVALID_HANDLE_VALUE;
        }
    }

    bool NamedPipeClient::IsConnected() const
    {
        return m_pipe != INVALID_HANDLE_VALUE;
    }

    std::optional<IpcMessage> NamedPipeClient::Send(const IpcMessage& request,
                                                     DWORD /*timeoutMs*/)
    {
        if (m_pipe == INVALID_HANDLE_VALUE)
            return std::nullopt;

        if (!Wire::Write(m_pipe, request))
        {
            Disconnect();
            return std::nullopt;
        }

        ::FlushFileBuffers(m_pipe);

        IpcMessage response;
        if (!Wire::Read(m_pipe, response))
        {
            Disconnect();
            return std::nullopt;
        }

        return response;
    }

    std::shared_ptr<IIpcClient> CreateNamedPipeClient()
    {
        return std::make_shared<NamedPipeClient>();
    }
}
