#pragma once
#include "App.xaml.g.h"

#include <winrt/Windows.ApplicationModel.Activation.h>

namespace winrt::UwpApp::implementation
{
    struct App : AppT<App>
    {
        App();
        void OnFileActivated(Windows::ApplicationModel::Activation::FileActivatedEventArgs const& args);
        void OnLaunched(Windows::ApplicationModel::Activation::LaunchActivatedEventArgs const&);
        void OnSuspending(IInspectable const&, Windows::ApplicationModel::SuspendingEventArgs const&);
        void OnNavigationFailed(IInspectable const&, Windows::UI::Xaml::Navigation::NavigationFailedEventArgs const&);

    private:
        void PerformLaunch(winrt::hstring const& arguments);
    };
}
