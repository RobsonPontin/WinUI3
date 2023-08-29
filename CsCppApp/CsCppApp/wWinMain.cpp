#include "pch.h"

#include "App.xaml.h"

#include <wil/resource.h>
#include <winrt/Microsoft.Windows.AppLifecycle.h>
#include <winrt/Windows.System.h>

#include "DotNetHost.h"

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, PWSTR lpCmdline, int /*nCmdShow*/)
{
    winrt::init_apartment(winrt::apartment_type::single_threaded);

    // TEST:
    //::CsCppApp::Service::DotNetHost dotNetHost;
    // dotNetHost.Load(hInstance);

    winrt::Microsoft::UI::Xaml::Application::Start([](auto&&)
        {
            ::winrt::make<winrt::CsCppApp::implementation::App>();
        });

    return 0;
}