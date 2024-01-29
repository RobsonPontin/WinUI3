#include "pch.h"

#include "App.xaml.h"

#include <winrt/Windows.System.h>
#include <wil/resource.h>

// Redirection vars
static winrt::event_token g_activationToken{};
static wil::unique_event g_redirectEventHandle{};
static WF::IAsyncAction g_redirectionActivationOp{ nullptr };

static winrt::fire_and_forget RedirectActivationTo(MWAL::AppInstance const& keyInstance, MWAL::AppActivationArguments const& args)
{
    g_redirectEventHandle.create();

    if (g_redirectionActivationOp != nullptr
        && g_redirectionActivationOp.Status() == WF::AsyncStatus::Started)
    {
        g_redirectionActivationOp.Cancel();
    }

    wil::event_set_scope_exit ensure_signaled = wil::SetEvent_scope_exit(g_redirectEventHandle.get());
    g_redirectionActivationOp = keyInstance.RedirectActivationToAsync(args);
    co_await g_redirectionActivationOp;
}

static WF::IAsyncAction KeyInstance_Activated(WF::IInspectable const& /*sender*/, MWAL::AppActivationArguments const& args)
{
    auto currentApplication = WUX::Application::Current();
    if (currentApplication != nullptr)
    {
        auto currentApp = currentApplication.try_as<winrt::SelfContainedDeployment::implementation::App>();
        WINRT_ASSERT(currentApp != nullptr && L"WUX::Application::Current cannot be casted to App class");
        if (currentApp != nullptr)
        {
            co_await currentApp.get()->HandleOnRedirectionActivatedAsync(args);
        }
    }

    co_return;
}

static bool DecideRedirection()
{
    bool isRedirect = false;

    try
    {
        MWAL::AppActivationArguments args{ nullptr };

        try
        {
            args = MWAL::AppInstance::GetCurrent().GetActivatedEventArgs();

            if (args.Kind() == MWAL::ExtendedActivationKind::ProtocolForResults)
            {
                return false;
            }
        }
        catch (...)
        {
            throw;
        }

        // Register or find an existing instance. There should only be one instance running.
        MWAL::AppInstance keyInstance = MWAL::AppInstance::FindOrRegisterForKey(L"mainInstance");

        // If we successfully registered the key, we must be the only instance running that was activated. 
        if (keyInstance.IsCurrent())
        {
            // Hook up the Activated event, to allow for this instance of the app
            // getting reactivated as a result of multi-instance redirection.
            g_activationToken = keyInstance.Activated(&KeyInstance_Activated);
        }
        else
        {
            isRedirect = true;

            try
            {
                args = MWAL::AppInstance::GetCurrent().GetActivatedEventArgs();
                RedirectActivationTo(keyInstance, args);

                DWORD handleIndex = 0;
                HANDLE rawHandle = g_redirectEventHandle.get();
                if (CoWaitForMultipleObjects(CWMO_DEFAULT, INFINITE, 1, &rawHandle, &handleIndex) != 0)
                {
                    // Fail
                }
            }
            catch (...)
            {
                // Fail
            }
        }
    }
    catch (...)
    {
        // Fail
    }

    return isRedirect;
}

int __stdcall wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, PWSTR lpCmdline, int /*nCmdShow*/)
{
    winrt::init_apartment(winrt::apartment_type::single_threaded);

    if (DecideRedirection() == false)
    {
        winrt::Microsoft::UI::Xaml::Application::Start([](auto&&)
            {
                ::winrt::make<winrt::SelfContainedDeployment::implementation::App>();
            });
    }
}