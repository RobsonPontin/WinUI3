// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "App.xaml.g.h"

namespace winrt::AutoPlayApp::implementation
{
    struct App : AppT<App>
    {
        App(winrt::hstring initCmdLine);

        void OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const&);

    private:
        winrt::Microsoft::UI::Xaml::Window window{ nullptr };
        winrt::hstring m_initArgs{ L"" };
    };
}
