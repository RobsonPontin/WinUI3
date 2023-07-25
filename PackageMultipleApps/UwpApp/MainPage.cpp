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

    Windows::Foundation::IAsyncAction MainPage::LaunchWinUi3_ClickHandler(IInspectable const&, RoutedEventArgs const&)
    {
        try 
        {
            if (Windows::Foundation::Metadata::ApiInformation::IsApiContractPresent(L"Windows.ApplicationModel.FullTrustAppContract", 1, 0))
            {
                // Attempt to launch the WinUI 3 application from here
                co_await winrt::Windows::ApplicationModel::FullTrustProcessLauncher::LaunchFullTrustProcessForCurrentAppAsync();
            }
        }
        catch (winrt::hresult_error const& error)
        {
            // NOTE: usually fails when ApplicationModel can't find the ".exe" specified in the appxmanifest.
        }
    }
}
