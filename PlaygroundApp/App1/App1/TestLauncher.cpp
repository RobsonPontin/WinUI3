#include "pch.h"
#include "TestLauncher.h"

#include <winrt/Windows.Storage.h>

#include <filesystem>

#include <wil/resource.h>
#include <wil/stl.h>
#include <wil/win32_helpers.h>

namespace Playground
{
    constexpr std::wstring_view PlaygroundExe{ L"Playground.exe" };

	void TestLauncher::StartWithShellApi()
	{
        // This is defined in the AppxManifest
        std::wstring processName = L"PlaygroundApp.exe";

        SHELLEXECUTEINFO sei{};
        sei.cbSize = sizeof(SHELLEXECUTEINFO);
        sei.fMask = SEE_MASK_DEFAULT;
        sei.lpVerb = L"open";
        sei.lpFile = processName.data();
        auto parameters = std::format(L"\"{}\"", L"My Paramemters");
        sei.lpParameters = parameters.c_str();
        sei.nShow = SW_SHOWNORMAL;
        if (!ShellExecuteEx(&sei))
        {
            // Failed.
        }
	}

    inline const std::wstring GetPath(std::wstring_view relativePath = L"")
    {
        std::filesystem::path path{ winrt::Windows::ApplicationModel::Package::Current().InstalledLocation().Path().data() };
        path = path / relativePath;

        return path;
    }

    void TestLauncher::StartWithWin32CreateProcessApi()
    {
        wil::unique_process_information _piClient;
        STARTUPINFOEX siEx{ 0 };
        siEx.StartupInfo.cb = sizeof(STARTUPINFOEX);

        auto cmdline = GetPath(PlaygroundExe);

        auto result = CreateProcessW(
            nullptr, //lpApplicationName
            cmdline.data(), // lpCommandLine
            nullptr, // lpProcessAttributes
            nullptr, // lpThreadAttributes
            false, // bInheritHandles
            EXTENDED_STARTUPINFO_PRESENT | CREATE_UNICODE_ENVIRONMENT, //dwCreationFlags
            nullptr, // lpEnvironment
            nullptr, // lpCurrentDirectory
            &siEx.StartupInfo, // lpStartupInfo
            &_piClient // lpProcessInformation
        );

        if (!result)
        {
            // Fail
            auto lastError = GetLastError();
        }
    }
}
