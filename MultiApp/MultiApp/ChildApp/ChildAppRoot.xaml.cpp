#include "pch.h"
#include "ChildAppRoot.xaml.h"
#include "ChildAppMainWindow.xaml.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::ChildApp::implementation
{
    App::App()
    {
#if defined _DEBUG && !defined DISABLE_XAML_GENERATED_BREAK_ON_UNHANDLED_EXCEPTION
        UnhandledException([](IInspectable const&, UnhandledExceptionEventArgs const& e)
        {
            if (IsDebuggerPresent())
            {
                auto errorMessage = e.Message();
                __debugbreak();
            }
        });
#endif
    }

    void App::OnLaunched([[maybe_unused]] LaunchActivatedEventArgs const& e)
    {
        window = make<ChildAppMainWindow>();
        window.Activate();
    }
}
