#include "pch.h"

#include "App.xaml.h"

#include <wil/resource.h>
#include <winrt/Microsoft.Windows.AppLifecycle.h>
#include <winrt/Windows.System.h>
#include "LifetimeManager.h"

wil::unique_event _shutdownEvent{ wil::EventOptions::None };

int __stdcall wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, PWSTR lpCmdline, int /*nCmdShow*/)
{
    /* NOTE: When creating a COM object.
     * 
     * at this point when a specific COM object is attempted to be instantiated, wWinMain will
     * be called and passed "-Embedding" as command line which comes from COM indicating this app should
     * register itself as a running COM server.
     *
     * If this returns "0" right away it will fail to start server on the COM class creation on the client.
     * 
     * References:
     * - https://github.com/microsoft/cppwinrt/issues/601
     * - https://learn.microsoft.com/en-us/windows/uwp/cpp-and-winrt-apis/author-coclasses
     */
    std::wstring cmdLineStr(lpCmdline);
    if (cmdLineStr == L"-Embedding")
    {
        //winrt::init_apartment(winrt::apartment_type::multi_threaded);

        // Initialize COM before any registration starts
        CoInitialize(nullptr);

        auto lifetimeManager = COMServerApp::LifetimeManager::CreateShared();
        lifetimeManager->RegisterComClass();

        /* Call CoWaitForMultipleObjects to wait for new message to come in
         * from other processes which will kick the instancing of COM objects.* /

        /* TODO: _shutdownEvent is still not getting released, it should be set() once all 
         * ref count of COM obj reached 0 and the process can be shutdown. */
        DWORD index{};
        HANDLE events[] = { lifetimeManager->GetSHutdownEventHandle()};
        winrt::check_hresult(CoWaitForMultipleObjects(CWMO_DISPATCH_CALLS | CWMO_DISPATCH_WINDOW_MESSAGES, INFINITE,
            static_cast<ULONG>(std::size(events)), events, &index));

        return S_OK;
    }

    winrt::init_apartment(winrt::apartment_type::single_threaded);

    winrt::Microsoft::UI::Xaml::Application::Start([](auto&&)
        {
            ::winrt::make<winrt::COMServerApp::implementation::App>();
        });

    return 0;
}