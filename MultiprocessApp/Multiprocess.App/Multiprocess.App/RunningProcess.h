#pragma once

#include <thread>

namespace Multiprocess
{
	namespace Core
	{
		struct RunningProcess
		{
			RunningProcess()
			{

			}

			void TryInitialize();

			STARTUPINFO StartInfo() const
			{
				return m_srtartupInfo;
			}

			PROCESS_INFORMATION ProcessInfo() const
			{
				return m_processInfo;
			}

		private:
			void CreateBackgroundServiceImpl();

			bool m_isRunning{ false };

			STARTUPINFO m_srtartupInfo;
			PROCESS_INFORMATION m_processInfo;

			std::thread m_backgroundService;
		};
	}
}