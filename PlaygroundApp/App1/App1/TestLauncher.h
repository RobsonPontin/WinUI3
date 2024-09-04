#pragma once

namespace Playground
{
	struct TestLauncher
	{
		TestLauncher() = default;

		void StartWithShellApi();
		void StartWithWin32CreateProcessApi();
		void StartWithActivationManager();

		/// <summary>
		/// Launching Microsoft Photos App
		/// https://learn.microsoft.com/en-us/windows/uwp/launch-resume/launch-default-app#photos-app-uri-scheme
		/// </summary>
		winrt::Windows::Foundation::IAsyncAction StartPhotosAppWithProtocolLaunchForFile(winrt::hstring const& filePath);

		/// <summary>
		/// This API does not work with WinUI 3 atm, only UWP
		/// thread: https://github.com/microsoft/WindowsAppSDK/issues/2856
		/// </summary>
		void StartWithActivationManagerForFile(winrt::hstring const& filePath);
	};
}
