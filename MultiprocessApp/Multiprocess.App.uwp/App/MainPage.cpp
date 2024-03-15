#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"

#include "winrt/Windows.Foundation.Metadata.h"
#include "winrt/Windows.ApplicationModel.h"

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::Multiprocess_App_uwp::implementation
{
    int32_t MainPage::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainPage::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void MainPage::ClickHandler(IInspectable const&, RoutedEventArgs const&)
    {
        try
        {
            if (Windows::Foundation::Metadata::ApiInformation::IsApiContractPresent(L"Windows.ApplicationModel.FullTrustAppContract", 1, 0))
            {
                // Param needs to match the GroupID defined in the AppxManifest
                winrt::Windows::ApplicationModel::FullTrustProcessLauncher::LaunchFullTrustProcessForCurrentAppAsync();
            }
        }
        catch (winrt::hresult_error const& ex)
        {
            // NOTE: usually fails when ApplicationModel can't find the ".exe" specified in the appxmanifest.
        }
    }
}
