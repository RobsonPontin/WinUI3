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
		/// This API does not work with WinUI 3 atm, only UWP
		/// thread: https://github.com/microsoft/WindowsAppSDK/issues/2856
		/// </summary>
		void StartWithActivationManagerForFile(winrt::hstring const& wsFilePath);
	};
}
