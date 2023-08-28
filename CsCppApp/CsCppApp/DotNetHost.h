#pragma once

namespace CsCppApp
{
	struct DotNetHost : std::enable_shared_from_this<DotNetHost>
	{
		bool Load();
	};
}
