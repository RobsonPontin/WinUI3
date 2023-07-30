/* NOTE: Sample app to explore authoring COM object with C++/WinRT and
 * hook it up events to AutoPlay.
 * 
 * Source: https://learn.microsoft.com/en-us/windows/uwp/cpp-and-winrt-apis/author-coclasses
 */

#include "pch.h"

#include "AutoPlayHandler.h"
#include "AppLauncher.h"
#include "InputParser.h"

using namespace winrt;
using namespace Windows::Foundation;
using namespace AutoPlayApp;

const winrt::hstring WINDOWS_PROTOCOL_AUTO_PLAY_APP { L"wp-autoplayapp:autoplay" };
const winrt::hstring AUTO_PLAY_APP_PACKAGE_NAME { L"Rpontin.Winui.AutoPlayApp_9yd5akztwvwhp" };

int main(int argc, char** argv)
{


    InputParser input(argc, argv);

    if (input.cmdOptionExists("-h"))
    {
        printf("No help for YOU!\n");
    }
    else if (input.cmdOptionExists("-s"))
    {
        // Setup COM registry

        init_apartment(); // auto init COM

        auto autoPlayMng = std::unique_ptr<AutoPlayHandlerRegManager>();

        autoPlayMng->register_autoPlayHandler();
        autoPlayMng->update_registry();
    }
    else if (input.cmdOptionExists("-ls"))
    {
        // Launch with prompt/shell

        AppLauncher::ShellLaunchApp(AUTO_PLAY_APP_PACKAGE_NAME);
    }
    else if (input.cmdOptionExists("-lami"))
    {
        // Launch with Application Manager Interface (COM)

        winrt::hstring appAumid = AUTO_PLAY_APP_PACKAGE_NAME + L"!AppId";
        AppLauncher::LaunchApp(appAumid.c_str());
    }
    else if (input.cmdOptionExists("-lp"))
    {
        // Launch with Protocol Activation

        auto protocolLaunchOp = AppLauncher::ProtocolLaunchURIAsync(AUTO_PLAY_APP_PACKAGE_NAME, WINDOWS_PROTOCOL_AUTO_PLAY_APP);
        protocolLaunchOp.get(); // thread blocking
    }

    return 0;
}
