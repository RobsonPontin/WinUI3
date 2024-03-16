#pragma once

#include <thread>
#include <pplawait.h>
#include <iostream>
#include "Logger.h"

namespace Multiprocess
{
	namespace Core
	{
		struct RunningProcess : std::enable_shared_from_this<RunningProcess>
		{
			RunningProcess(){ }
			~RunningProcess() 
			{
				Utils::Logger::Debug("~RunningProcess()\n");
			}

			void TryInitialize();

			void SetProcessStartedCallback(std::function<void(std::shared_ptr<RunningProcess> process)> startedCallback);
			void SetProcessShutdownCallback(std::function<void(std::shared_ptr<RunningProcess> process)> shutdownCallback);

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

			std::thread m_backgroundService;
			std::mutex m_mutex;

			STARTUPINFO m_srtartupInfo;
			PROCESS_INFORMATION m_processInfo;

			// Event callbacks
			std::function<void(std::shared_ptr<RunningProcess> process)> m_processStartedCallback;
			std::function<void(std::shared_ptr<RunningProcess> process)> m_processShutdownCallback;
		};
	}
}