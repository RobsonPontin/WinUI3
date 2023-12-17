// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"

#include "App.xaml.h"
#include "MainWindow.xaml.h"

#include <winrt/Microsoft.Windows.AppLifecycle.h>

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
    using namespace Windows::ApplicationModel::Activation;
    using namespace winrt::Microsoft::Windows::AppLifecycle;
    using namespace winrt::Windows::ApplicationModel::Activation;

    auto activatedArgs = AppInstance::AppInstance::GetCurrent().GetActivatedEventArgs();
    auto iActivatedArgs = activatedArgs.Data().try_as<IActivatedEventArgs>();

    if (iActivatedArgs.Kind() == ActivationKind::Protocol)
    {
        auto protocol = iActivatedArgs.try_as<IProtocolActivatedEventArgsWithCallerPackageFamilyNameAndData>();
		if (protocol != nullptr && protocol.Data() != nullptr)
		{
			auto argsData = protocol.Data().Lookup(L"args");
			if (argsData != nullptr)
			{
                // Get all the arguments from protocol launch including the DeviceId from Autoplay "IHWEventHandler2::HandleEventWithHWND"
				auto argsDataStr = argsData.as<hstring>();
			}
		}        
    }

    // kick off Window creation
    window = make<MainWindow>();
    window.Title(L"Init command line: " + m_initArgs);
    window.Activate();
}