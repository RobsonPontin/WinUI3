#pragma once

// Shared protocol definitions for the MultiApp ↔ ChildApp handshake.
// Both projects include this header via ProcessBridge so the "contract" stays in sync.

#include "ProcessBridgeApi.h"

#include <string>
#include <string_view>
#include <windows.h>

namespace ProcessBridge::Protocol
{
    // Command-line switch prefixes passed from parent to child.
    constexpr std::wstring_view kSwitchEvent = L"--mp-validation-event=";
    constexpr std::wstring_view kSwitchPid   = L"--mp-validation-pid=";

    // Named-event prefix used by the parent when creating the validation event.
    constexpr std::wstring_view kEventNamePrefix = L"Local\\MultiAppChildLaunchValidation-";

    // Default validation timeout (milliseconds). Consumers may override.
    constexpr DWORD kDefaultValidationTimeoutMs = 2000;

    // Named pipe prefix for IPC between parent and child.
    // Full pipe name is: kPipeNamePrefix + <launch-token>
    constexpr std::wstring_view kPipeNamePrefix = L"\\\\.\\pipe\\MultiAppIpc-";

    // Command-line switch for passing the pipe name to the child.
    constexpr std::wstring_view kSwitchPipeName = L"--mp-ipc-pipe=";

    // Returns the directory containing the currently running executable,
    // including the trailing backslash. Empty on failure.
    PROCESSBRIDGE_API std::wstring GetOwnDirectory();

    // Searches argv for a switch with the given prefix and returns the value
    // portion (everything after the prefix). Returns false if not found.
    PROCESSBRIDGE_API bool TryGetSwitchValue(int argc, LPWSTR* argv,
                                             std::wstring_view prefix,
                                             std::wstring& out_value);
}
