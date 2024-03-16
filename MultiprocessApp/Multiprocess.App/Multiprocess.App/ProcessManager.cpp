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
			m_requestedProcesses = std::make_shared<std::vector<std::shared_ptr<RunningProcess>>>();
        }

		void ProcessManager::CreateBackgroundService()
		{
			std::shared_ptr<RunningProcess> process = std::make_shared<RunningProcess>();

			process->SetProcessStartedCallback([=](std::shared_ptr<RunningProcess> process)
				{
					m_activeProcesses->push_back(process);

					m_requestedProcesses->erase(
						std::remove(m_requestedProcesses->begin(), m_requestedProcesses->end(), process), 
						m_requestedProcesses->end());

					if (m_processStartedCallback != nullptr)
					{
						m_processStartedCallback(process->shared_from_this());
					}
				});

			process->SetProcessShutdownCallback([=](std::shared_ptr<RunningProcess> process)
				{
					m_activeProcesses->erase(
						std::remove(m_activeProcesses->begin(), m_activeProcesses->end(), process),
						m_activeProcesses->end());

					if (m_processShutdownCallback != nullptr)
					{
						m_processShutdownCallback(process->shared_from_this());
					}
				});

			m_requestedProcesses->push_back(process);
			process->TryInitialize();
		}

		// ## Events/Callback for notification. ##

		void ProcessManager::SetProcessStartedCallback(std::function<void(std::shared_ptr<RunningProcess> process)> startedCallback)
		{
			m_processStartedCallback = startedCallback;
		}

		void ProcessManager::SetProcessShutdownCallback(std::function<void(std::shared_ptr<RunningProcess> process)> shutdownCallback)
		{
			m_processShutdownCallback = shutdownCallback;
		}
	}
}