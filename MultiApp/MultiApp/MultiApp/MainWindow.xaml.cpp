#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include <bcrypt.h>
#include <string>

#pragma comment(lib, "bcrypt.lib")

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace
{
    // Switch / event naming for the parent-identity handshake. Different from the
    // Edge "share-validation-*" pair so the two protocols don't collide.
    constexpr wchar_t kEventNamePrefix[] = L"Local\\MultiAppChildLaunchValidation-";
    constexpr wchar_t kSwitchEvent[]     = L"--mp-validation-event=";
    constexpr wchar_t kSwitchPid[]       = L"--mp-validation-pid=";
    constexpr DWORD kValidationTimeoutMs = 2000;

    // 128-bit random hex string. Unguessable so a third party can't open the
    // event by name and falsely signal validation.
    std::wstring GenerateLaunchToken()
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
}

namespace winrt::MultiApp::implementation
{
    void MainWindow::LaunchChildButton_Click(
        winrt::Windows::Foundation::IInspectable const& /*sender*/,
        winrt::Microsoft::UI::Xaml::RoutedEventArgs const& /*args*/)
    {
        wchar_t exePath[MAX_PATH]{};
        DWORD len = ::GetModuleFileNameW(nullptr, exePath, MAX_PATH);
        if (len == 0 || len == MAX_PATH)
        {
            StatusText().Text(L"Failed to resolve module path.");
            return;
        }

        std::wstring childPath{ exePath };
        size_t slash = childPath.find_last_of(L"\\/");
        if (slash != std::wstring::npos)
        {
            childPath.resize(slash + 1);
        }
        childPath += L"ChildApp.exe";

        // Parent-identity handshake: create a named event with an unguessable
        // token. The child opens it by name and SetEvents it once it has
        // verified that our image path is the expected parent EXE.
        std::wstring token = GenerateLaunchToken();
        if (token.empty())
        {
            StatusText().Text(L"Failed to generate launch token.");
            return;
        }
        std::wstring eventName = kEventNamePrefix + token;

        HANDLE validationEvent = ::CreateEventW(
            /*lpEventAttributes=*/nullptr,
            /*bManualReset=*/TRUE,
            /*bInitialState=*/FALSE,
            eventName.c_str());
        if (!validationEvent)
        {
            StatusText().Text(L"CreateEventW failed (error " +
                              std::to_wstring(::GetLastError()) + L").");
            return;
        }

        DWORD parentPid = ::GetCurrentProcessId();

        // CreateProcessW requires a writable command-line buffer. Embed both
        // switches; the child parses them out of GetCommandLineW().
        std::wstring commandLine = L"\"" + childPath + L"\"";
        commandLine += L" ";
        commandLine += kSwitchEvent;
        commandLine += eventName;
        commandLine += L" ";
        commandLine += kSwitchPid;
        commandLine += std::to_wstring(parentPid);

        STARTUPINFOW si{};
        si.cb = sizeof(si);
        PROCESS_INFORMATION pi{};

        BOOL ok = ::CreateProcessW(
            childPath.c_str(),
            commandLine.data(),
            nullptr,
            nullptr,
            FALSE,
            0,
            nullptr,
            nullptr,
            &si,
            &pi);

        if (!ok)
        {
            DWORD err = ::GetLastError();
            ::CloseHandle(validationEvent);
            StatusText().Text(L"CreateProcessW failed (error " +
                              std::to_wstring(err) + L").");
            return;
        }

        ::CloseHandle(pi.hThread);

        // Wait on either (a) the child signalling validation succeeded, or
        // (b) the child exiting first — that means it rejected the launch.
        HANDLE waits[2] = { validationEvent, pi.hProcess };
        DWORD waitResult = ::WaitForMultipleObjects(
            2, waits, /*bWaitAll=*/FALSE, kValidationTimeoutMs);

        std::wstring status;
        switch (waitResult)
        {
        case WAIT_OBJECT_0:
            status = L"ChildApp validated parent identity (PID " +
                     std::to_wstring(pi.dwProcessId) + L").";
            break;
        case WAIT_OBJECT_0 + 1:
            status = L"ChildApp exited before signalling validation (rejected launch).";
            break;
        case WAIT_TIMEOUT:
            status = L"ChildApp validation timed out.";
            break;
        default:
            status = L"WaitForMultipleObjects failed (" +
                     std::to_wstring(::GetLastError()) + L").";
            break;
        }
        StatusText().Text(status);

        ::CloseHandle(validationEvent);
        ::CloseHandle(pi.hProcess);
    }
}
