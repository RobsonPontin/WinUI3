// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"

#include "App2.xaml.h"
#include "MainWindow.xaml.h"

#include "winrt/Windows.Storage.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::UI::Xaml::Navigation;
using namespace WinUi3App;
using namespace WinUi3App::implementation;

winrt::hstring _lastActivationType = L"";

/// <summary>
/// Initializes the singleton application object.  This is the first line of authored code
/// executed, and as such is the logical equivalent of main() or WinMain().
/// </summary>
App2::App2()
{
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

App2::App2(winrt::hstring const& activationType)
{
    InitializeComponent();

    _lastActivationType = activationType;

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
void App2::OnLaunched(LaunchActivatedEventArgs const&)
{
    std::optional<hstring> lastFileActivated;
    if (_lastActivationType == L"FileActivation")
    {
        // Attempt to retrieve last cached file in the UWP App to App Local settings:
        auto localSettings = winrt::Windows::Storage::ApplicationData::Current().LocalSettings();
        auto lastFileActivation = localSettings.Values().TryLookup(L"LastFileActivationKey");
        if (lastFileActivation != nullptr)
        {
            lastFileActivated = lastFileActivation.try_as<winrt::hstring>();
        }
    }
    else if (_lastActivationType == L"LaunchActivation")
    {
        // Normal launch, do nothing.
    }

    if (lastFileActivated.has_value())
    {
        window = make<MainWindow>(lastFileActivated.value());
    }
    {
        window = make<MainWindow>();
    }

    window.Activate();
}
