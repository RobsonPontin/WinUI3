#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"

#include "winrt//Windows.Foundation.Metadata.h"

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::UwpApp::implementation
{
    int32_t MainPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    Windows::Foundation::IAsyncAction LaunchProcessAsync(winrt::hstring const& param = L"")
    {
        try
        {
            if (Windows::Foundation::Metadata::ApiInformation::IsApiContractPresent(L"Windows.ApplicationModel.FullTrustAppContract", 1, 0))
            {
                if (param == L"")
                {
                    co_await winrt::Windows::ApplicationModel::FullTrustProcessLauncher::LaunchFullTrustProcessForCurrentAppAsync();
                }
                else
                {
                    co_await winrt::Windows::ApplicationModel::FullTrustProcessLauncher::LaunchFullTrustProcessForCurrentAppAsync(param);
                }
            }
        }
        catch (winrt::hresult_error const& error)
        {
            // NOTE: usually fails when ApplicationModel can't find the ".exe" specified in the appxmanifest.
        }
    }

    Windows::Foundation::IAsyncAction MainPage::LaunchWinUi3_ClickHandler(IInspectable const&, RoutedEventArgs const&)
    {
        co_await LaunchProcessAsync();
    }

    Windows::Foundation::IAsyncAction MainPage::btnLaunchParam1_ClickHandler(IInspectable const&, RoutedEventArgs const&)
    {
        // Needs to match the GroupID defined in the AppxManifest
        co_await LaunchProcessAsync(L"LaunchActivation");
    }

    Windows::Foundation::IAsyncAction MainPage::btnLaunchParam2_ClickHandler(IInspectable const&, RoutedEventArgs const&)
    {
        // Needs to match the GroupID defined in the AppxManifest
        co_await LaunchProcessAsync(L"FileActivation");
    }
}
