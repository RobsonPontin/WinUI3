#include "pch.h"
#include "FullTrustAppLauncher.h"

#include "winrt/Windows.Foundation.Metadata.h"

namespace winrt::UwpApp
{
    const hstring APP_GROUP_ID_LAUNCH_ACTIVATION = L"LaunchActivation";
    const hstring APP_GROUP_ID_FILE_ACTIVATION = L"FileActivation";

	// forward declaration
    Windows::Foundation::IAsyncAction PerformLaunchActivationAsync(winrt::hstring const param);

	void FullTrustAppLauncher::PerformLaunchActivation()
	{
        PerformLaunchActivationAsync(APP_GROUP_ID_LAUNCH_ACTIVATION);
	}

	void FullTrustAppLauncher::PerformFileActivation()
	{
        PerformLaunchActivationAsync(APP_GROUP_ID_FILE_ACTIVATION);
	}

	Windows::Foundation::IAsyncAction PerformLaunchActivationAsync(winrt::hstring const param)
	{
        try
        {
            if (Windows::Foundation::Metadata::ApiInformation::IsApiContractPresent(L"Windows.ApplicationModel.FullTrustAppContract", 1, 0))
            {
                // Param needs to match the GroupID defined in the AppxManifest
                co_await winrt::Windows::ApplicationModel::FullTrustProcessLauncher::LaunchFullTrustProcessForCurrentAppAsync(param);
            }
        }
        catch (winrt::hresult_error const&)
        {
            // NOTE: usually fails when ApplicationModel can't find the ".exe" specified in the appxmanifest.
        }
	}
}