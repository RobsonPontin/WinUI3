#include "pch.h"
#include "AppLauncher.h"

#include "winrt/Windows.System.h"

namespace Multiprocess
{
    winrt::Windows::Foundation::IAsyncAction AppLauncher::ProtocolLaunchURIAsync(
        winrt::hstring const& packageFamilyName,
        winrt::hstring const& commandLineUri,
        winrt::hstring const& args)
    {
        winrt::Windows::Foundation::Uri packageFamiliyNameUri{ commandLineUri };

        winrt::Windows::System::LauncherOptions options;
        options.TargetApplicationPackageFamilyName(packageFamilyName);

        winrt::Windows::Foundation::Collections::ValueSet inputData;
        inputData.Insert(L"args", winrt::box_value(args));

        co_await winrt::Windows::System::Launcher::LaunchUriAsync(packageFamiliyNameUri, options, inputData);
    }
}