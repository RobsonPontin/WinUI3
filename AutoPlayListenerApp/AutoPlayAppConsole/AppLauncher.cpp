#include "pch.h"
#include "AppLauncher.h"

#include "winrt/Windows.System.h"

#include <ShObjIdl.h>
#include <atlbase.h>

HRESULT AppLauncher::LaunchApp(LPCWSTR AUMID)
{
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (FAILED(hr))
    {
        wprintf(L"LaunchApp %s: Failed to init COM. hr = 0x%08lx \n", AUMID, hr);
    }
    {
        CComPtr<IApplicationActivationManager> AppActivationMgr = nullptr;
        if (SUCCEEDED(hr))
        {
            hr = CoCreateInstance(CLSID_ApplicationActivationManager, nullptr,
                CLSCTX_LOCAL_SERVER, IID_PPV_ARGS(&AppActivationMgr));
            if (FAILED(hr))
            {
                wprintf(L"LaunchApp %s: Failed to create Application Activation Manager.hr = 0x % 08lx \n", AUMID, hr);
            }
        }
        if (SUCCEEDED(hr))
        {
            DWORD pid = 0;
            hr = AppActivationMgr->ActivateApplication(AUMID, nullptr, AO_NONE, &pid);
            if (FAILED(hr))
            {
                wprintf(L"LaunchApp %s: Failed to Activate App. hr = 0x%08lx \n", AUMID, hr);
            }
        }
    }
    CoUninitialize();
    return hr;
}

void AppLauncher::ShellLaunchApp(winrt::hstring const& packageFamilyName)
{
    winrt::hstring cmd = L"explorer.exe shell::AppsFolder\\" + packageFamilyName + L"!App";
    const char* cmd_ptr = winrt::to_string(cmd).c_str();
    system(cmd_ptr);
}

winrt::Windows::Foundation::IAsyncAction AppLauncher::ProtocolLaunchURIAsync(
    winrt::hstring const& packageFamilyName, 
    winrt::hstring const& commandLineUri)
{
    winrt::Windows::Foundation::Uri packageFamiliyNameUri { commandLineUri };

    winrt::Windows::System::LauncherOptions options;
    options.TargetApplicationPackageFamilyName(packageFamilyName);

    winrt::Windows::Foundation::Collections::ValueSet inputData;
    inputData.Insert(L"activation", winrt::box_value(L"autoPlay"));

    co_await winrt::Windows::System::Launcher::LaunchUriAsync(packageFamiliyNameUri, options, inputData);

    co_return;
}