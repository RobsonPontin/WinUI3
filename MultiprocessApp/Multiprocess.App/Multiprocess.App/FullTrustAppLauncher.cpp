#include "pch.h"
#include "FullTrustAppLauncher.h"

#include "winrt/Windows.Foundation.Metadata.h"
#include "winrt/Windows.ApplicationModel.h"

namespace winrt::Multiprocess::App
{
    const hstring APP_GROUP_ID_BACKGROUND_SERVICE = L"BackgroundService";

	// forward declaration
    Windows::Foundation::IAsyncAction LaunchServiceAsync(winrt::hstring const param);

	void FullTrustAppLauncher::LaunchService()
	{
        LaunchServiceAsync(APP_GROUP_ID_BACKGROUND_SERVICE);
	}

	Windows::Foundation::IAsyncAction LaunchServiceAsync(winrt::hstring const param)
	{
        try
        {
            if (Windows::Foundation::Metadata::ApiInformation::IsApiContractPresent(L"Windows.ApplicationModel.FullTrustAppContract", 1, 0))
            {
                // Param needs to match the GroupID defined in the AppxManifest
                co_await winrt::Windows::ApplicationModel::FullTrustProcessLauncher::LaunchFullTrustProcessForCurrentAppAsync(param);
            }
        }
        catch (winrt::hresult_error const& ex)
        {
            // NOTE: usually fails when ApplicationModel can't find the ".exe" specified in the appxmanifest.
        }
	}
}