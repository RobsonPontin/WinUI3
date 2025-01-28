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

auto create_my_server_instance()
{
    auto guid = winrt::guid_of<MyServer>();    
    return winrt::create_instance<winrt::Windows::Foundation::IStringable>(guid, CLSCTX_LOCAL_SERVER);
}

void HandleModuleNotifier()
{
    _shutdownEvent.SetEvent();
}

int __stdcall wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, PWSTR lpCmdline, int /*nCmdShow*/)
{
	using namespace std::chrono;

    wil::notifiable_module_lock::instance().set_notifier(&HandleModuleNotifier);

    winrt::init_apartment();

    const auto startTime = std::chrono::system_clock::now();

    auto revoker = wil::register_com_server<MyServer>();

    _shutdownEvent.wait();

    const auto endTime = system_clock::now();
    const auto totalTimeRunning = duration_cast<milliseconds>(startTime - endTime);

    return 0;
}