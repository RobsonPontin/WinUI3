#pragma once

#include <winrt/Windows.Storage.h>

namespace Playground
{
	struct TestApplicationData
	{
        TestApplicationData() = default;

        void Init();
        void Test();

    private:
        winrt::Windows::Storage::ApplicationData m_applicationData{ nullptr };

        std::thread m_thread1;
        std::thread m_thread2;
        std::thread m_thread3;
        std::thread m_thread4;
        std::thread m_thread5;
	};
}