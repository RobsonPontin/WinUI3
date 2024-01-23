#pragma once

#include "UserControl.g.h"

namespace winrt::LibXaml::implementation
{
    struct UserControl : UserControlT<UserControl>
    {
        UserControl()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void myButton_Click(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::LibXaml::factory_implementation
{
    struct UserControl : UserControlT<UserControl, implementation::UserControl>
    {
    };
}
