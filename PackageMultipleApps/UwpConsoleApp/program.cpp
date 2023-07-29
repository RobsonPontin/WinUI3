#include "pch.h"

using namespace winrt;

int main()
{
    // Attempt to launch the WinUI 3 application from here
    winrt::Windows::ApplicationModel::FullTrustProcessLauncher::LaunchFullTrustProcessForCurrentAppAsync();

    // TODO: temp leave the process alive for 5 sec but need a loop here to check of WinUI 3 app is still running.
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(5s);

    return 0;
}
