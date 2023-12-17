#pragma once

#include "pch.h"

#include "App.xaml.h"
#include <wil/resource.h>
#include <winrt/Microsoft.Windows.AppLifecycle.h>
#include <winrt/Windows.System.h>

int __stdcall wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, PWSTR lpCmdline, int /*nCmdShow*/)
{
    winrt::init_apartment(winrt::apartment_type::single_threaded);

    // NOTE: if activated by command line we can get arguments, example using alias: "AutoPlayAppCL /start" 
    std::wstring cmdLineStr(lpCmdline);

    winrt::Microsoft::UI::Xaml::Application::Start([cmdLineStr](auto&&)
        {
            ::winrt::make<winrt::AutoPlayApp::implementation::App>(winrt::to_hstring(cmdLineStr.c_str()));
        });

    return 0;
}