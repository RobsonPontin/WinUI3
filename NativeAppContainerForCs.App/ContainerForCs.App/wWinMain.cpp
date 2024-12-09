#include "pch.h"

#include <wil/resource.h>
#include <winrt/Microsoft.Windows.AppLifecycle.h>
#include <winrt/Windows.System.h>

#include <../ContainerForCs.AppLib/AppFactory.h>

int __stdcall wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, PWSTR /*lpCmdline*/, int /*nCmdShow*/)
{
    winrt::init_apartment(winrt::apartment_type::single_threaded);

    winrt::Microsoft::UI::Xaml::Application::Start([](auto&&)
        {
            ::ContainerForCs::AppLib::AppFactory::CreateApp();
        });

    return 0;
}