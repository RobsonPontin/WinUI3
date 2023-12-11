#include "pch.h"

#include "App.xaml.h"

#include <wil/resource.h>
#include <winrt/Microsoft.Windows.AppLifecycle.h>
#include <winrt/Windows.System.h>

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, PWSTR lpCmdline, int /*nCmdShow*/)
{
    /* NOTE: at this point when "COMServerAppSimpleClass" is instantiated it will call
     * wWinMain and pass "-Embedding" as command line arguments, if this returns "0" right
     * away it will fail to start server on the COM class creation. 
     * 
     * TODO: how this could be handled? do we need a message loop to pump here? (ie. "CAtlExeModuleT::WinMain").
     */
    std::wstring cmdLineStr(lpCmdline);
    if (cmdLineStr == L"-Embedding")
    {
        return 0;
    }

    winrt::init_apartment(winrt::apartment_type::single_threaded);

    winrt::Microsoft::UI::Xaml::Application::Start([](auto&&)
        {
            ::winrt::make<winrt::COMServerApp::implementation::App>();
        });

    return 0;
}