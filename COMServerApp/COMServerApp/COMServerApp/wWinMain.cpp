#include "pch.h"

#define WINRT_CUSTOM_MODULE_LOCK
#include <functional>
#include <wil/cppwinrt_notifiable_module_lock.h>
#include <winrt/Windows.Foundation.h>
#include <wil/cppwinrt_register_com_server.h>
#include <wil/resource.h>
#include <string_view>
#include <thread>
#include <chrono>
#include <random>

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/COM.ServerLib.h>

wil::unique_event _shutdownEvent{ wil::EventOptions::None };

struct MyServer : winrt::implements<MyServer, winrt::Windows::Foundation::IStringable>
{
    MyServer()
    {
    }

    int RandomNumberGenerator()
    {
        // Create a random device to seed the random number generator
        std::random_device rd;

        // Use the Mersenne Twister engine
        std::mt19937 gen(rd());

        // Define the range for the random numbers
        std::uniform_int_distribution<> distr(1, 100);

        // Generate a random number
        return distr(gen);
    }

    winrt::hstring ToString()
    {
        return L"MyServer hello. Random #: " + winrt::to_hstring(RandomNumberGenerator());
    }
};

struct MyCustomServer : winrt::implements<MyCustomServer, winrt::COM::ServerLib::IProcessCommunication>
{
    MyCustomServer()
    {
    }

    bool IsThisWorking()
    {
        return true;
    }

	winrt::Windows::Foundation::Collections::IVector<winrt::hstring> FilePaths()
	{
		winrt::Windows::Foundation::Collections::IVector<winrt::hstring> paths = winrt::single_threaded_vector<winrt::hstring>();
		paths.Append(L"Path1");
		paths.Append(L"Path2");
        paths.Append(L"Path3");
		paths.Append(L"Path4");
		paths.Append(L"Path5");
		paths.Append(L"Path6");
		paths.Append(L"Path7");

		return paths;
	}
};

void HandleModuleNotifier()
{
    _shutdownEvent.SetEvent();
}

int __stdcall wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, PWSTR /*lpCmdline*/, int /*nCmdShow*/)
{
    winrt::init_apartment();

    using namespace std::chrono;

    wil::notifiable_module_lock::instance().set_notifier(&HandleModuleNotifier);

    const auto startTime = std::chrono::system_clock::now();

    auto revoker = wil::register_com_server<MyServer>();
    auto revoker2 = wil::register_com_server<MyCustomServer>();

    _shutdownEvent.wait();

    const auto endTime = system_clock::now();
    const auto totalTimeRunning = duration_cast<milliseconds>(startTime - endTime);

    return 0;
}