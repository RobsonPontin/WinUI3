#pragma once

#include "ChildAppMainWindow.g.h"

namespace winrt::ChildApp::implementation
{
    struct ChildAppMainWindow : ChildAppMainWindowT<ChildAppMainWindow>
    {
        ChildAppMainWindow()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }
    };
}

namespace winrt::ChildApp::factory_implementation
{
    struct ChildAppMainWindow : ChildAppMainWindowT<ChildAppMainWindow, implementation::ChildAppMainWindow>
    {
    };
}
