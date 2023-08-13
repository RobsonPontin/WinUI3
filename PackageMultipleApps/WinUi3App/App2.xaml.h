// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "App2.xaml.g.h"

namespace winrt::WinUi3App::implementation
{
    // NOTE: renamed "App" to "App2" since during deployment of package it conflicted with "UWP App".
    struct App2 : AppT<App2>
    {
        App2();

        void OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const&);

    private:
        winrt::Microsoft::UI::Xaml::Window window{ nullptr };

        void MergeResource(hstring const& resourcePath);
    };
}
