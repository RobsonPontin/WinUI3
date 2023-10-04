#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"

#include "FullTrustAppLauncher.h"

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

    void MainPage::LaunchWinUi3_ClickHandler(IInspectable const&, RoutedEventArgs const&)
    {
        FullTrustAppLauncher::PerformLaunchActivation();
    }

    void MainPage::btnLaunchParam1_ClickHandler(IInspectable const&, RoutedEventArgs const&)
    {
        FullTrustAppLauncher::PerformLaunchActivation();
    }

    void MainPage::btnLaunchParam2_ClickHandler(IInspectable const&, RoutedEventArgs const&)
    {
        // Needs to match the GroupID defined in the AppxManifest
        FullTrustAppLauncher::PerformFileActivation();
    }
}
