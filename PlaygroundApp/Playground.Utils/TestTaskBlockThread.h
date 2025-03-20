#pragma once

#include <iostream>

#include "ITask.h"

namespace Playground::Utils
{
    // Simple Test task which blocks the thread for 2 seconds.
    struct TestTaskBlockThread : ::Playground::Utils::ITask
    {
        TestTaskBlockThread(::Playground::Utils::TaskPriority priority)
        {
            m_priority = priority;
        }

        ::Playground::Utils::TaskPriority Priority()
        {
            return m_priority;
        }

        bool Execute()
        {
            std::cout << "Task - Starting delay..." << std::endl;

            // TODO: after sleep, it will resume to a different background thread
            std::this_thread::sleep_for(std::chrono::seconds(2));
            std::cout << "Task - delay finished." << std::endl;

            return true;
        }

    private:
        ::Playground::Utils::TaskPriority m_priority{ ::Playground::Utils::TaskPriority::Low };
    };
}
