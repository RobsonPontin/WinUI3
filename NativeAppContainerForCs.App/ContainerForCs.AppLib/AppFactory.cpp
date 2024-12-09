#include "pch.h"
#include "AppFactory.h"

#include "App.xaml.h"

namespace ContainerForCs::AppLib
{
    /*static*/ winrt::Microsoft::UI::Xaml::Application AppFactory::CreateApp()
    {
        return ::winrt::make<winrt::ContainerForCs::AppLib::implementation::App>();
    }
}