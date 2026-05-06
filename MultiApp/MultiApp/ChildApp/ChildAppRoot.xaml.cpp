#include "pch.h"
#include "ChildAppRoot.xaml.h"
#include "ChildAppMainWindow.xaml.h"

#include <shellapi.h>
#include <memory>
#include <string>
#include <string_view>

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace
{
    // Must match the switch / event naming used by MultiApp's
    // LaunchChildButton_Click handshake.
    constexpr std::wstring_view kSwitchEvent = L"--mp-validation-event=";
    constexpr std::wstring_view kSwitchPid   = L"--mp-validation-pid=";
    constexpr wchar_t kExpectedParentExe[]   = L"MultiApp.exe";

    bool TryGetSwitchValue(int argc, LPWSTR* argv, std::wstring_view prefix,
                           std::wstring& out_value)
    {
        for (int i = 1; i < argc; ++i)
        {
            std::wstring_view arg{ argv[i] };
            if (arg.size() >= prefix.size() &&
                arg.compare(0, prefix.size(), prefix) == 0)
            {
                out_value.assign(arg.substr(prefix.size()));
                return true;
            }
        }
        return false;
    }

    std::wstring GetProcessImagePath(HANDLE process)
    {
        wchar_t buf[MAX_PATH]{};
        DWORD size = MAX_PATH;
        if (!::QueryFullProcessImageNameW(process, /*dwFlags=*/0, buf, &size))
        {
            return {};
        }
        return std::wstring{ buf, size };
    }

    std::wstring GetOwnDirectory()
    {
        wchar_t self[MAX_PATH]{};
        DWORD len = ::GetModuleFileNameW(nullptr, self, MAX_PATH);
        if (len == 0 || len == MAX_PATH)
        {
            return {};
        }
        std::wstring path{ self, len };
        size_t slash = path.find_last_of(L"\\/");
        if (slash == std::wstring::npos)
        {
            return {};
        }
        path.resize(slash + 1);
        return path;
    }

    // Verify that this process was activated by a co-located MultiApp.exe and
    // signal the parent's named event so it knows the handshake succeeded.
    // Returns true only when the parent is legitimate. Any failure (missing
    // switches, parent gone, image-path mismatch, event open failure) is
    // treated as an unauthorised launch.
    bool ValidateParentLaunch()
    {
        int argc = 0;
        LPWSTR* argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);
        if (!argv)
        {
            return false;
        }
        struct ArgvDeleter { void operator()(LPWSTR* p) const noexcept { ::LocalFree(p); } };
        std::unique_ptr<LPWSTR[], ArgvDeleter> argvOwner{ argv };

        std::wstring eventName;
        std::wstring pidStr;
        if (!TryGetSwitchValue(argc, argv, kSwitchEvent, eventName) ||
            !TryGetSwitchValue(argc, argv, kSwitchPid, pidStr) ||
            eventName.empty() || pidStr.empty())
        {
            return false;
        }

        DWORD parentPid = 0;
        try
        {
            unsigned long parsed = std::stoul(pidStr);
            parentPid = static_cast<DWORD>(parsed);
        }
        catch (...)
        {
            return false;
        }
        if (parentPid == 0)
        {
            return false;
        }

        HANDLE parent = ::OpenProcess(
            PROCESS_QUERY_LIMITED_INFORMATION, FALSE, parentPid);
        if (!parent)
        {
            return false;
        }
        std::wstring parentExe = GetProcessImagePath(parent);
        ::CloseHandle(parent);
        if (parentExe.empty())
        {
            return false;
        }

        // Expected parent: MultiApp.exe sitting next to our own EXE in the
        // package install directory. Co-location proves the parent ships in
        // the same package as us, since CreateProcessW used a full path.
        std::wstring ownDir = GetOwnDirectory();
        if (ownDir.empty())
        {
            return false;
        }
        std::wstring expectedParent = ownDir + kExpectedParentExe;

        if (::CompareStringOrdinal(
                parentExe.c_str(), -1,
                expectedParent.c_str(), -1,
                /*bIgnoreCase=*/TRUE) != CSTR_EQUAL)
        {
            return false;
        }

        HANDLE validationEvent = ::OpenEventW(
            EVENT_MODIFY_STATE, FALSE, eventName.c_str());
        if (!validationEvent)
        {
            return false;
        }
        BOOL signalled = ::SetEvent(validationEvent);
        ::CloseHandle(validationEvent);
        return signalled != FALSE;
    }
}

namespace winrt::ChildApp::implementation
{
    App::App()
    {
        // Parent-identity handshake. If the launch wasn't initiated by the
        // co-located MultiApp.exe with matching switches, terminate before
        // we touch any UI. ExitProcess is intentional: anything more graceful
        // (Application::Exit etc.) would require XAML to be initialised first.
        if (!ValidateParentLaunch())
        {
            ::ExitProcess(1);
        }

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
        UnhandledException([](IInspectable const&, UnhandledExceptionEventArgs const& e)
        {
            if (IsDebuggerPresent())
            {
                auto errorMessage = e.Message();
                __debugbreak();
            }
        });
#endif
    }

    void App::OnLaunched([[maybe_unused]] LaunchActivatedEventArgs const& e)
    {
        window = make<ChildAppMainWindow>();
        window.Activate();
    }
}
