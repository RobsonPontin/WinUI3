#include "pch.h"

#include "App.h"
#include "MainPage.h"

#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#include <winrt/Windows.Storage.h>

using namespace winrt;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Foundation;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Navigation;
using namespace UwpApp;
using namespace UwpApp::implementation;

#define ENABLE_START_WINUI_3_APP 0

/// <summary>
/// Creates the singleton application object.  This is the first line of authored code
/// executed, and as such is the logical equivalent of main() or WinMain().
/// </summary>
App::App()
{
    Suspending({ this, &App::OnSuspending });

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

void App::OnFileActivated(Windows::ApplicationModel::Activation::FileActivatedEventArgs const& e)
{
    // Get and save activation type/kind to local settings
    if (e.Kind() == Windows::ApplicationModel::Activation::ActivationKind::File)
    {
        auto fileActArgs = e.as<Windows::ApplicationModel::Activation::FileActivatedEventArgs>();
        auto file = fileActArgs.Files().GetAt(0);

        auto localSettings = winrt::Windows::Storage::ApplicationData::Current().LocalSettings();
        localSettings.Values().Insert(L"LastFileActivationKey", winrt::box_value(file.Path()));
    }
    
    PerformLaunch(L"");
}

/// <summary>
/// Invoked when the application is launched normally by the end user.  Other entry points
/// will be used such as when the application is launched to open a specific file.
/// </summary>
/// <param name="e">Details about the launch request and process.</param>
void App::OnLaunched(LaunchActivatedEventArgs const& e)
{
    PerformLaunch(e.Arguments());
}

void App::PerformLaunch(winrt::hstring const& arguments)
{
#if(ENABLE_START_WINUI_3_APP)

    _tprintf(TEXT("!!!!!!!!!!!!!!!!!!!!!!!!!!!!OnLaunched.\n"));

    // Attempt to launch the WinUI 3 application from here
    winrt::Windows::ApplicationModel::FullTrustProcessLauncher::LaunchFullTrustProcessForCurrentAppAsync();

    /* Wait some time for confirmation of UWP launched succefully and then quite the application.
     * The problem with this approach is that once we call "Application::Current().Exit()" the
     * "RuntimeBroker.exe" will keep running even after closing the WinUI 3 app.
     */
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(50ms);

    Application::Current().Exit();
#else
    Frame rootFrame{ nullptr };
    auto content = Window::Current().Content();
    if (content)
    {
        rootFrame = content.try_as<Frame>();
    }

    // Do not repeat app initialization when the Window already has content,
    // just ensure that the window is active
    if (rootFrame == nullptr)
    {
        // Create a Frame to act as the navigation context and associate it with
        // a SuspensionManager key
        rootFrame = Frame();

        rootFrame.NavigationFailed({ this, &App::OnNavigationFailed });

		if (rootFrame.Content() == nullptr)
		{
			// When the navigation stack isn't restored navigate to the first page,
			// configuring the new page by passing required information as a navigation
			// parameter
			rootFrame.Navigate(xaml_typename<UwpApp::MainPage>(), box_value(arguments));
		}
		// Place the frame in the current Window
		Window::Current().Content(rootFrame);
		// Ensure the current window is active
		Window::Current().Activate();        
    }
    else
    {
		if (rootFrame.Content() == nullptr)
		{
			// When the navigation stack isn't restored navigate to the first page,
			// configuring the new page by passing required information as a navigation
			// parameter
			rootFrame.Navigate(xaml_typename<UwpApp::MainPage>(), box_value(arguments));
		}
		// Ensure the current window is active
		Window::Current().Activate();
    }
#endif
}

/// <summary>
/// Invoked when application execution is being suspended.  Application state is saved
/// without knowing whether the application will be terminated or resumed with the contents
/// of memory still intact.
/// </summary>
/// <param name="sender">The source of the suspend request.</param>
/// <param name="e">Details about the suspend request.</param>
void App::OnSuspending([[maybe_unused]] IInspectable const& sender, [[maybe_unused]] SuspendingEventArgs const& e)
{
    // Save application state and stop any background activity
}

/// <summary>
/// Invoked when Navigation to a certain page fails
/// </summary>
/// <param name="sender">The Frame which failed navigation</param>
/// <param name="e">Details about the navigation failure</param>
void App::OnNavigationFailed(IInspectable const&, NavigationFailedEventArgs const& e)
{
    throw hresult_error(E_FAIL, hstring(L"Failed to load Page ") + e.SourcePageType().Name);
}

int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
    _tprintf(TEXT("!!!!!!!!!!!!!!!!!!!!!!!!!!!!wWinMain.\n"));

    Windows::UI::Xaml::Application::Start([](auto&&) { ::winrt::make<winrt::UwpApp::implementation::App>(); });

    return 0;
}