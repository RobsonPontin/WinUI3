#pragma once

#include "App.xaml.g.h"

namespace winrt::Multiprocess::App::implementation
{
    struct App : AppT<App>
    {
        App();

        void OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const&);
        WF::IAsyncAction PerformProcessRedirectionAsync(MWAL::AppActivationArguments e);

    private:
        winrt::Microsoft::UI::Xaml::Window window{ nullptr };
    };
}
