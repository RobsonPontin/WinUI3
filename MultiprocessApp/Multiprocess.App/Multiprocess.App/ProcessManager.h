#pragma once

#include "RunningProcess.h"

namespace Multiprocess
{
	namespace Core 
	{
		struct ProcessManager
		{
			ProcessManager();

			void CreateBackgroundService();

			std::shared_ptr<std::vector<std::shared_ptr<RunningProcess>>> RunningProcesses()
			{
				return m_activeProcesses;
			}

		private:
			std::shared_ptr<std::vector<std::shared_ptr<RunningProcess>>> m_activeProcesses;
		};
	}
}