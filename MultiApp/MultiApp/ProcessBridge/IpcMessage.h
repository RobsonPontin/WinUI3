#pragma once

#include "ProcessBridgeApi.h"

#include <string>
#include <cstdint>
#include <windows.h>

namespace ProcessBridge::Ipc
{
    struct IpcMessage
    {
        std::wstring command;
        std::wstring payload;
    };

    // Simple binary wire format:
    //   [4 bytes: command length in chars]
    //   [N wchar_t: command]
    //   [4 bytes: payload length in chars]
    //   [M wchar_t: payload]

    namespace Wire
    {
        PROCESSBRIDGE_API bool Write(HANDLE pipe, const IpcMessage& msg);
        PROCESSBRIDGE_API bool Read(HANDLE pipe, IpcMessage& msg);
    }
}
