#include "pch.h"

#include "App.xaml.h"

#include <winrt/Windows.System.h>
#include <wil/resource.h>

// Forward declaration
std::vector<std::wstring> split_string(const std::wstring& wstr, char delimiter);
static bool DecideRedirection(winrt::hstring const&);
static winrt::fire_and_forget RedirectActivationTo(MWAL::AppInstance const& keyInstance, MWAL::AppActivationArguments const& args);
static WF::IAsyncAction KeyInstance_Activated(WF::IInspectable const& /*sender*/, MWAL::AppActivationArguments const& args);

// Redirection vars
static winrt::event_token g_activationToken{};
static wil::unique_event g_redirectEventHandle{};
static WF::IAsyncAction g_redirectionActivationOp{ nullptr };

int __stdcall wWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, PWSTR lpCmdline, int /*nCmdShow*/)
{
    UNREFERENCED_PARAMETER(hInstance);

    winrt::init_apartment(winrt::apartment_type::single_threaded);

    auto cmdLineStr = std::wstring(lpCmdline);
    if (cmdLineStr != L"")
    {        
        auto result = split_string(std::wstring(cmdLineStr), ':');
        auto hstrCmdlineResult = winrt::hstring(result.at(1));
        auto shouldRedirect = DecideRedirection(hstrCmdlineResult);
    }
    else
    {
        /* When registering a Process instance for the first time we will
         * register an event for Activation to handle redirection requests. 
         * It would be nicessary for each Process Instance Type:
         * - Main
         * - Spare
         * - Viewer
         */

        auto mainInstance = MWAL::AppInstance::FindOrRegisterForKey(L"main");
        if (mainInstance.IsCurrent())
        {
            g_activationToken = mainInstance.Activated(&KeyInstance_Activated);
        }

        winrt::Microsoft::UI::Xaml::Application::Start([](auto&&)
            {
                ::winrt::make<winrt::Multiprocess::App::implementation::App>();
            });
    }

    return 0;
}

std::vector<std::wstring> split_string(const std::wstring& wstr, char delimiter)
{
    std::vector<std::wstring> result;
    size_t start = 0;
    size_t end = wstr.find(delimiter);

    while (end != std::string::npos)
    {
        result.push_back(wstr.substr(start, end - start));
        start = end + 1;
        end = wstr.find(delimiter, start);
    }
    result.push_back(wstr.substr(start, end));
    return result;
}

static bool DecideRedirection(winrt::hstring const& processName)
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

        MWAL::AppInstance keyInstance = MWAL::AppInstance::FindOrRegisterForKey(processName);
        
        if (!keyInstance.IsCurrent())
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
        auto currentApp = currentApplication.try_as<winrt::Multiprocess::App::implementation::App>();
        WINRT_ASSERT(currentApp != nullptr && L"WUX::Application::Current cannot be casted to App class");
        if (currentApp != nullptr)
        {
            co_await currentApp.get()->PerformProcessRedirectionAsync(args);
        }
    }

    co_return;
}