#include "pch.h"

#include "App.xaml.h"

#include <winrt/Microsoft.Windows.AppLifecycle.h>
#include <winrt/Windows.System.h>

int __stdcall wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, PWSTR lpCmdline, int /*nCmdShow*/)
{
    winrt::init_apartment(winrt::apartment_type::single_threaded);

    winrt::Microsoft::UI::Xaml::Application::Start([](auto&&)
        {
            ::winrt::make<winrt::Playground::implementation::App>();
        });

    return 0;
}