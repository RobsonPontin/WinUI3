#pragma once

#include <winnt.h>

namespace ComServerExe
{
	struct AppLauncher
	{
		static HRESULT StartWithParam(PCWSTR pszParams);
	};
}