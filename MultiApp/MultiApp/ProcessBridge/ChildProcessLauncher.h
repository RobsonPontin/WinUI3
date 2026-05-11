#pragma once

#include "IChildProcessLauncher.h"
#include "ProcessBridgeProtocol.h"

namespace ProcessBridge
{
    struct ChildProcessLauncherOptions
    {
        // Required: the executable name of the child process to launch.
        std::wstring childExeName;

        std::wstring eventNamePrefix{ Protocol::kEventNamePrefix };
        std::wstring switchEvent{ Protocol::kSwitchEvent };
        std::wstring switchPid{ Protocol::kSwitchPid };
        DWORD validationTimeoutMs{ Protocol::kDefaultValidationTimeoutMs };
    };

    class ChildProcessLauncher : public IChildProcessLauncher
    {
    public:
        PROCESSBRIDGE_API explicit ChildProcessLauncher(ChildProcessLauncherOptions options = {});
        PROCESSBRIDGE_API LaunchResult Launch() override;

    private:
        std::wstring GenerateLaunchToken() const;
        std::wstring ResolveChildPath() const;

        ChildProcessLauncherOptions m_options;
    };

    // Factory function for creating the default launcher.
    // childExeName is required (e.g., L"ChildApp.exe").
    PROCESSBRIDGE_API std::shared_ptr<IChildProcessLauncher> CreateChildProcessLauncher(
        ChildProcessLauncherOptions options);
}
