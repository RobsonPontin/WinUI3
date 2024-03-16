#pragma once

#include "ProcessRunner.h"

namespace Multiprocess
{
	namespace Core 
	{
		struct ProcessManager
		{
			ProcessManager();

			void CreateBackgroundService();

		private:
			std::shared_ptr<std::vector<std::shared_ptr<ProcessRunner>>> m_activeProcesses;
		};
	}
}