#pragma once

#include "UserControlCommon.g.h"

namespace winrt::LibXaml::Common::implementation
{
    struct UserControlCommon : UserControlCommonT<UserControlCommon>
    {
        UserControlCommon()
        {
            // Xaml objects should not call InitializeComponent during construction.
            // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent
        }

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void myButton_Click(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
    };
}

namespace winrt::LibXaml::Common::factory_implementation
{
    struct UserControlCommon : UserControlCommonT<UserControlCommon, implementation::UserControlCommon>
    {
    };
}
