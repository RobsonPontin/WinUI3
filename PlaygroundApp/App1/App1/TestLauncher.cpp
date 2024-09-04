#include "pch.h"
#include "TestLauncher.h"

#include <winrt/Windows.Storage.h>
#include <winrt/Windows.System.h>

#include <atlbase.h>
#include <filesystem>
#include <ShObjIdl.h>

#include <wil/resource.h>
#include <wil/stl.h>
#include <wil/win32_helpers.h>

namespace Playground
{
    constexpr std::wstring_view PlaygroundExe{ L"Playground.exe" };
    const winrt::hstring PLAYGROUND_APP_PACKAGE_NAME{ L"Rpontin.Winui.PlaygroundApp_9yd5akztwvwhp" };
    constexpr std::wstring_view MICROSOFT_PHOTOS_PACKAGE_NAME = L"Microsoft.Windows.Photos_8wekyb3d8bbwe";

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

    void TestLauncher::StartWithActivationManager()
    {
        HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
        if (FAILED(hr))
        {
            wprintf(L"LaunchApp %s: Failed to init COM. hr = 0x%08lx \n", PLAYGROUND_APP_PACKAGE_NAME.c_str(), hr);
            return;
        }
        
        CComPtr<IApplicationActivationManager> appActivationMgr = nullptr;
        if (SUCCEEDED(hr))
        {
            hr = CoCreateInstance(
                CLSID_ApplicationActivationManager,
                nullptr,
                CLSCTX_LOCAL_SERVER,
                IID_PPV_ARGS(&appActivationMgr));

            if (FAILED(hr))
            {
                wprintf(L"LaunchApp %s: Failed to create Application Activation Manager.hr = 0x % 08lx \n", PLAYGROUND_APP_PACKAGE_NAME.c_str(), hr);
            }
        }

        if (SUCCEEDED(hr))
        {
            winrt::hstring aumid = PLAYGROUND_APP_PACKAGE_NAME + L"!App";
            winrt::hstring arguments = L"my arguments test";

            DWORD pid = 0;
            hr = appActivationMgr->ActivateApplication(aumid.c_str(), arguments.c_str(), AO_NONE, &pid);
            if (FAILED(hr))
            {
                wprintf(L"LaunchApp %s: Failed to Activate App. hr = 0x%08lx \n", PLAYGROUND_APP_PACKAGE_NAME.c_str(), hr);
            }
        }
        
        CoUninitialize();
    }

    winrt::Windows::Foundation::IAsyncAction TestLauncher::StartPhotosAppWithProtocolLaunchForFile(winrt::hstring const& filePath)
    {
        winrt::hstring uriSchema = L"ms-photos:viewer?fileName=" + filePath;
        winrt::Windows::Foundation::Uri photosUri{ uriSchema };
        winrt::Windows::System::LauncherOptions options;
        options.TargetApplicationPackageFamilyName(MICROSOFT_PHOTOS_PACKAGE_NAME);

        co_await winrt::Windows::System::Launcher::LaunchUriAsync(photosUri, options, nullptr);
    }

    void TestLauncher::StartWithActivationManagerForFile(winrt::hstring const& filePath)
    {
        HRESULT result = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
        if (SUCCEEDED(result))
        {
            CComPtr<IApplicationActivationManager> appActivationMgr = nullptr;
            result = CoCreateInstance(
                CLSID_ApplicationActivationManager,
                NULL,
                CLSCTX_LOCAL_SERVER,
                IID_IApplicationActivationManager,
                (LPVOID*)&appActivationMgr);

            if (SUCCEEDED(result))
            {
                IShellItem* pShellItem = NULL;
                IShellItemArray* pShellItemArray = NULL;

                HRESULT hr = SHCreateItemFromParsingName(filePath.c_str(), NULL, IID_PPV_ARGS(&pShellItem));
                if (hr == S_OK && pShellItem != NULL)
                {
                    hr = SHCreateShellItemArrayFromShellItem(pShellItem, IID_IShellItemArray, (VOID**)&pShellItemArray);
                    if (hr == S_OK && pShellItemArray != NULL)
                    {
                        DWORD pid = 0;
                        winrt::hstring appAumid = PLAYGROUND_APP_PACKAGE_NAME + L"!App";

                        // NOTE: at this point the whole app will fail crash
                        hr = appActivationMgr->ActivateForFile(appAumid.c_str(), pShellItemArray, NULL, &pid);
                        if (FAILED(hr))
                        {
                            // Fail
                        }
                    }
                }
            }

            CoUninitialize();
        }
    }
}
