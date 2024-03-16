#include "pch.h"

#include "ProcessManager.h"

#include <stdio.h>
#include <tchar.h>

namespace Multiprocess
{
	namespace Core
	{
        ProcessManager::ProcessManager()
        {
            m_activeProcesses = std::make_shared<std::vector<std::shared_ptr<RunningProcess>>>();
        }

		void ProcessManager::CreateBackgroundService()
		{
			std::shared_ptr<RunningProcess> process = std::make_shared<RunningProcess>();
            process->TryInitialize();

			// Ideally we only add to ActiveProcesses once the Process is confirmed to be running, so
			// we need to listen to an event to tells us about it, or monitor within Manager?
			m_activeProcesses->push_back(process);
		}
	}
}