#pragma once

namespace winrt::UwpApp
{
	struct FullTrustAppLauncher
	{
		static void PerformLaunchActivation();
		static void PerformFileActivation();
	};
}