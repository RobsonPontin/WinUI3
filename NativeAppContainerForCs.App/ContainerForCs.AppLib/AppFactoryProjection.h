#pragma once

#include "AppFactory.g.h"

namespace winrt::ContainerForCs::AppLib::implementation
{
    struct AppFactory : AppFactoryT<AppFactory>
    {
        static Microsoft::UI::Xaml::Application CreateApp();
    };
}

namespace winrt::ContainerForCs::AppLib::factory_implementation
{
    struct AppFactory : AppFactoryT<AppFactory, implementation::AppFactory>
    {
    };
}
