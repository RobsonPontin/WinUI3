#include "pch.h"
#include "AppFactoryProjection.h"

#include "AppFactory.g.cpp"

#include "AppFactory.h"

namespace winrt::ContainerForCs::AppLib::implementation
{
    /*static*/ Microsoft::UI::Xaml::Application AppFactory::CreateApp()
    {
        return ::ContainerForCs::AppLib::AppFactory::CreateApp();
    }
}
