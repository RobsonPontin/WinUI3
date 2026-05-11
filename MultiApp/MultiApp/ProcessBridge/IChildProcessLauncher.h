#pragma once

#include "ProcessBridgeApi.h"

#include <string>
#include <memory>
#include <windows.h>

namespace ProcessBridge
{
    struct LaunchResult
    {
        bool succeeded{ false };
        DWORD childProcessId{ 0 };
        std::wstring pipeName;
        std::wstring message;
    };

    // Interface for launching and validating child processes.
    // Enables dependency injection and testability.
    struct IChildProcessLauncher
    {
        virtual ~IChildProcessLauncher() = default;
        virtual LaunchResult Launch() = 0;
    };
}
