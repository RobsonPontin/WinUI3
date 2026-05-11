#include "pch.h"
#include "ChildProcessLauncher.h"

#include <bcrypt.h>

#pragma comment(lib, "bcrypt.lib")

namespace ProcessBridge
{
    ChildProcessLauncher::ChildProcessLauncher(ChildProcessLauncherOptions options)
        : m_options(std::move(options))
    {
    }

    std::wstring ChildProcessLauncher::GenerateLaunchToken() const
    {
        unsigned char bytes[16]{};
        if (!BCRYPT_SUCCESS(::BCryptGenRandom(
                nullptr, bytes, sizeof(bytes), BCRYPT_USE_SYSTEM_PREFERRED_RNG)))
        {
            return {};
        }
        wchar_t hex[sizeof(bytes) * 2 + 1]{};
        for (size_t i = 0; i < sizeof(bytes); ++i)
        {
            ::swprintf_s(&hex[i * 2], 3, L"%02x", bytes[i]);
        }
        return hex;
    }

    std::wstring ChildProcessLauncher::ResolveChildPath() const
    {
        std::wstring dir = Protocol::GetOwnDirectory();
        if (dir.empty())
        {
            return {};
        }
        return dir + m_options.childExeName;
    }

    LaunchResult ChildProcessLauncher::Launch()
    {
        // Resolve child executable path relative to our own directory.
        std::wstring childPath = ResolveChildPath();
        if (childPath.empty())
        {
            return { .succeeded = false, .message = L"Failed to resolve module path." };
        }

        // Generate unguessable token for the parent-identity handshake.
        std::wstring token = GenerateLaunchToken();
        if (token.empty())
        {
            return { .succeeded = false, .message = L"Failed to generate launch token." };
        }

        std::wstring eventName = m_options.eventNamePrefix + token;

        // Create the named event the child will signal after validating us.
        HANDLE validationEvent = ::CreateEventW(
            nullptr, TRUE, FALSE, eventName.c_str());
        if (!validationEvent)
        {
            return { .succeeded = false, .message = L"CreateEventW failed (error " +
                               std::to_wstring(::GetLastError()) + L")." };
        }

        DWORD parentPid = ::GetCurrentProcessId();

        // Build command line with validation switches.
        std::wstring pipeName = std::wstring{ Protocol::kPipeNamePrefix } + token;
        std::wstring commandLine = L"\"" + childPath + L"\"";
        commandLine += L" " + m_options.switchEvent + eventName;
        commandLine += L" " + m_options.switchPid + std::to_wstring(parentPid);
        commandLine += L" " + std::wstring{ Protocol::kSwitchPipeName } + pipeName;

        STARTUPINFOW si{};
        si.cb = sizeof(si);
        PROCESS_INFORMATION pi{};

        BOOL ok = ::CreateProcessW(
            childPath.c_str(),
            commandLine.data(),
            nullptr, nullptr,
            FALSE, 0,
            nullptr, nullptr,
            &si, &pi);

        if (!ok)
        {
            DWORD err = ::GetLastError();
            ::CloseHandle(validationEvent);
            return { .succeeded = false, .message = L"CreateProcessW failed (error " +
                               std::to_wstring(err) + L")." };
        }

        ::CloseHandle(pi.hThread);

        // Wait for validation signal or child exit.
        HANDLE waits[2] = { validationEvent, pi.hProcess };
        DWORD waitResult = ::WaitForMultipleObjects(
            2, waits, FALSE, m_options.validationTimeoutMs);

        LaunchResult result;
        result.childProcessId = pi.dwProcessId;
        result.pipeName = pipeName;

        switch (waitResult)
        {
        case WAIT_OBJECT_0:
            result.succeeded = true;
            result.message = L"ChildApp validated parent identity (PID " +
                             std::to_wstring(pi.dwProcessId) + L").";
            break;
        case WAIT_OBJECT_0 + 1:
            result.message = L"ChildApp exited before signalling validation (rejected launch).";
            break;
        case WAIT_TIMEOUT:
            result.message = L"ChildApp validation timed out.";
            break;
        default:
            result.message = L"WaitForMultipleObjects failed (" +
                             std::to_wstring(::GetLastError()) + L").";
            break;
        }

        ::CloseHandle(validationEvent);
        ::CloseHandle(pi.hProcess);

        return result;
    }

    std::shared_ptr<IChildProcessLauncher> CreateChildProcessLauncher(
        ChildProcessLauncherOptions options)
    {
        return std::make_shared<ChildProcessLauncher>(std::move(options));
    }
}
