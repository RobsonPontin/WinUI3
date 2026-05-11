#include "pch.h"
#include "IpcMessage.h"

namespace ProcessBridge::Ipc::Wire
{
    bool Write(HANDLE pipe, const IpcMessage& msg)
    {
        auto writeU32 = [&](uint32_t val) -> bool {
            DWORD written = 0;
            return ::WriteFile(pipe, &val, sizeof(val), &written, nullptr) &&
                   written == sizeof(val);
        };
        auto writeStr = [&](const std::wstring& s) -> bool {
            uint32_t len = static_cast<uint32_t>(s.size());
            if (!writeU32(len))
                return false;
            if (len == 0)
                return true;
            DWORD bytes = len * sizeof(wchar_t);
            DWORD written = 0;
            return ::WriteFile(pipe, s.data(), bytes, &written, nullptr) &&
                   written == bytes;
        };
        return writeStr(msg.command) && writeStr(msg.payload);
    }

    bool Read(HANDLE pipe, IpcMessage& msg)
    {
        auto readU32 = [&](uint32_t& val) -> bool {
            DWORD read = 0;
            return ::ReadFile(pipe, &val, sizeof(val), &read, nullptr) &&
                   read == sizeof(val);
        };
        auto readStr = [&](std::wstring& s) -> bool {
            uint32_t len = 0;
            if (!readU32(len))
                return false;
            if (len == 0)
            {
                s.clear();
                return true;
            }
            s.resize(len);
            DWORD bytes = len * sizeof(wchar_t);
            DWORD read = 0;
            return ::ReadFile(pipe, s.data(), bytes, &read, nullptr) &&
                   read == bytes;
        };
        return readStr(msg.command) && readStr(msg.payload);
    }
}
