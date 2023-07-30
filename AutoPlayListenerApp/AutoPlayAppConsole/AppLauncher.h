#pragma once

struct AppLauncher
{
	static HRESULT LaunchApp(LPCWSTR AUMID);

    static void ShellLaunchApp(winrt::hstring const& packageFamilyName);

    static winrt::Windows::Foundation::IAsyncAction ProtocolLaunchURIAsync(
        winrt::hstring const& packageFamilyName,
        winrt::hstring const& commandLineUri);
};