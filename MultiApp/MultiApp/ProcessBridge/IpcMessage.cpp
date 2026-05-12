#include "pch.h"
#include "IpcMessage.h"

namespace ProcessBridge::Ipc::Wire
{
    // Maximum allowed string length in characters.
    // Prevents a malformed length prefix from causing a huge allocation.
    constexpr uint32_t kMaxStringChars = 64 * 1024; // 64K chars = 128 KB

    // Writes exactly `totalBytes` to the pipe, looping over partial writes.
    static bool WriteAll(HANDLE pipe, const void* data, DWORD totalBytes)
    {
        const auto* ptr = static_cast<const BYTE*>(data);
        DWORD remaining = totalBytes;

        while (remaining > 0)
        {
            DWORD written = 0;
            if (!::WriteFile(pipe, ptr, remaining, &written, nullptr) || written == 0)
                return false;

            ptr += written;
            remaining -= written;
        }
        return true;
    }

    // Reads exactly `totalBytes` from the pipe, looping over partial reads.
    static bool ReadAll(HANDLE pipe, void* data, DWORD totalBytes)
    {
        auto* ptr = static_cast<BYTE*>(data);
        DWORD remaining = totalBytes;

        while (remaining > 0)
        {
            DWORD bytesRead = 0;
            if (!::ReadFile(pipe, ptr, remaining, &bytesRead, nullptr) || bytesRead == 0)
                return false;

            ptr += bytesRead;
            remaining -= bytesRead;
        }
        return true;
    }

    bool Write(HANDLE pipe, const IpcMessage& msg)
    {
        auto writeStr = [&](const std::wstring& s) -> bool {
            if (s.size() > kMaxStringChars)
                return false;

            uint32_t len = static_cast<uint32_t>(s.size());
            if (!WriteAll(pipe, &len, sizeof(len)))
                return false;

            if (len == 0)
                return true;

            DWORD bytes = len * static_cast<DWORD>(sizeof(wchar_t));
            return WriteAll(pipe, s.data(), bytes);
        };

        return writeStr(msg.command) && writeStr(msg.payload);
    }

    bool Read(HANDLE pipe, IpcMessage& msg)
    {
        auto readStr = [&](std::wstring& s) -> bool {
            uint32_t len = 0;
            if (!ReadAll(pipe, &len, sizeof(len)))
                return false;

            if (len > kMaxStringChars)
                return false; // Reject suspiciously large payloads.

            if (len == 0)
            {
                s.clear();
                return true;
            }

            s.resize(len);
            DWORD bytes = len * static_cast<DWORD>(sizeof(wchar_t));
            return ReadAll(pipe, s.data(), bytes);
        };

        return readStr(msg.command) && readStr(msg.payload);
    }
}
