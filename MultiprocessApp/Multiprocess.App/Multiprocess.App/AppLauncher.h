#pragma once

namespace Multiprocess
{
    struct AppLauncher
    {
        static winrt::Windows::Foundation::IAsyncAction ProtocolLaunchURIAsync(
            winrt::hstring const& packageFamilyName,
            winrt::hstring const& commandLineUri,
            winrt::hstring const& args);
    };
}