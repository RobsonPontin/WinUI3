// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"

#include "App.xaml.h"
#include "MainWindow.xaml.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::UI::Xaml::Navigation;
using namespace AutoPlayApp;
using namespace AutoPlayApp::implementation;

/// <summary>
/// Initializes the singleton application object.  This is the first line of authored code
/// executed, and as such is the logical equivalent of main() or WinMain().
/// </summary>
App::App(winrt::hstring initCmdLine)
{
    m_initArgs = initCmdLine;

    InitializeComponent();

#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
    UnhandledException([this](IInspectable const&, UnhandledExceptionEventArgs const& e)
    {
        if (IsDebuggerPresent())
        {
            auto errorMessage = e.Message();
            __debugbreak();
        }
    });
#endif
}

/// <summary>
/// Invoked when the application is launched.
/// </summary>
/// <param name="e">Details about the launch request and process.</param>
void App::OnLaunched(LaunchActivatedEventArgs const& args)
{
    window = make<MainWindow>();
    window.Title(L"Init command line: " + m_initArgs);
    window.Activate();
}

int __stdcall wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, PWSTR lpCmdline, int /*nCmdShow*/)
{
    winrt::init_apartment(winrt::apartment_type::single_threaded);      

    // NOTE: if activated by command line we can get arguments, example using alias: "AutoPlayAppCL /start" 
    std::wstring cmdLineStr(lpCmdline);

    Microsoft::UI::Xaml::Application::Start([cmdLineStr](auto&&)
        { 
            ::winrt::make<App>(winrt::to_hstring(cmdLineStr.c_str()));
        });
}