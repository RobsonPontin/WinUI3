#pragma once

namespace CsCppApp::Service
{
	struct DotNetHost : std::enable_shared_from_this<DotNetHost>
	{
		DotNetHost() = default;

		bool Load();
	};
}
