#pragma once

namespace Playground
{
	struct TestLauncher
	{
		TestLauncher() = default;

		void StartWithShellApi();
		void StartWithWin32CreateProcessApi();
	};
}
