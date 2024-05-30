#include "pch.h"
#include "TestApplicationData.h"

#include "DebugLog.h"

namespace Playground
{
    void TestApplicationData::Init()
    {
        DebugLog debugLog(L"ApplicationData_Init");
        m_applicationData = winrt::Windows::Storage::ApplicationData::Current();
        debugLog.Stop();
    }

	void TestApplicationData::Test()
	{
        if (!m_applicationData)
        {
            return;
        }

        m_thread1 = std::thread([this]()
            {
                this->m_applicationData.LocalSettings().CreateContainer(L"container1", winrt::Windows::Storage::ApplicationDataCreateDisposition::Always);
                auto localSettings = winrt::Windows::Storage::ApplicationData::Current().LocalSettings().Values();
                auto value = localSettings.TryLookup(L"test1");
                if (value)
                {
                }

                localSettings.Insert(L"test1", winrt::box_value(L"empty"));
            });
        m_thread1.detach();

        m_thread2 = std::thread([this]()
            {
                this->m_applicationData.LocalSettings().CreateContainer(L"container1", winrt::Windows::Storage::ApplicationDataCreateDisposition::Always);
                auto localSettings = winrt::Windows::Storage::ApplicationData::Current().LocalSettings().Values();
                auto value = localSettings.TryLookup(L"test1");
                if (value)
                {
                }

                localSettings.Insert(L"test1", winrt::box_value(L"empty"));
            });
        m_thread2.detach();

        m_thread3 = std::thread([this]()
            {
                this->m_applicationData.LocalSettings().CreateContainer(L"container1", winrt::Windows::Storage::ApplicationDataCreateDisposition::Always);
                auto localSettings = winrt::Windows::Storage::ApplicationData::Current().LocalSettings().Values();
                auto value = localSettings.TryLookup(L"test1");
                if (value)
                {
                }

                localSettings.Insert(L"test1", winrt::box_value(L"empty"));
            });
        m_thread3.detach();

        m_thread4 = std::thread([this]()
            {
                this->m_applicationData.LocalSettings().CreateContainer(L"container1", winrt::Windows::Storage::ApplicationDataCreateDisposition::Always);
                auto localSettings = winrt::Windows::Storage::ApplicationData::Current().LocalSettings().Values();
                auto value = localSettings.TryLookup(L"test1");
                if (value)
                {
                }

                localSettings.Insert(L"test1", winrt::box_value(L"empty"));
            });
        m_thread4.detach();

        m_thread5 = std::thread([this]()
            {
                this->m_applicationData.LocalSettings().CreateContainer(L"container1", winrt::Windows::Storage::ApplicationDataCreateDisposition::Always);
                auto localSettings = winrt::Windows::Storage::ApplicationData::Current().LocalSettings().Values();
                auto value = localSettings.TryLookup(L"test1");
                if (value)
                {
                }

                localSettings.Insert(L"test1", winrt::box_value(L"empty"));
            });

        m_thread5.detach();
	}

}