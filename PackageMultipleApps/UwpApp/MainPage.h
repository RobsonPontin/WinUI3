#pragma once

#include "MainPage.g.h"

namespace winrt::UwpApp::implementation
{
    struct MainPage : MainPageT<MainPage>
    {
        MainPage()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void LaunchWinUi3_ClickHandler(IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
        void btnLaunchParam1_ClickHandler(IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
        void btnLaunchParam2_ClickHandler(IInspectable const& sender, Windows::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::UwpApp::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
