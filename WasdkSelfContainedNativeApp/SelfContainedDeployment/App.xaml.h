// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#pragma once
#include "App.xaml.g.h"
#include "pch.h"

namespace winrt::SelfContainedDeployment::implementation
{
    struct App : AppT<App>
    {
        App();

        void OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const&);

        WF::IAsyncAction HandleOnRedirectionActivatedAsync(MWAL::AppActivationArguments e);
        
    private:
        Microsoft::UI::Xaml::Window window{ nullptr };
    };
}
