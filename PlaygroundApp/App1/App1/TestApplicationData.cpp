#include "pch.h"
#include "TestApplicationData.h"

namespace Playground
{
    DebugLogRecord TestApplicationData::Init()
    {
        DebugLog debugLog(L"ApplicationData_Init");
        m_applicationData = winrt::Windows::Storage::ApplicationData::Current();
        debugLog.Stop();

        return debugLog.LogRecord();
    }

    DebugLogRecord TestApplicationData::Write1000StringEntries()
    {
        if (!m_applicationData)
        {
            Init();
        }

        DebugLog debugLog(L"ApplicationData_Write1000StringEntries");

        winrt::hstring keyName = L"key";
        winrt::hstring value = L"my_value";
        for (int i = 0; i < 1000; ++i)
        {
            auto newkeyName = keyName + winrt::to_hstring(i);
            m_applicationData.LocalSettings().Values().Insert(newkeyName, winrt::box_value(value));
        }

        debugLog.Stop();
        return debugLog.LogRecord();
    }

    DebugLogRecord TestApplicationData::Read1000StringEntries()
    {
        if (!m_applicationData)
        {
            Init();
        }

        DebugLog debugLog(L"ApplicationData_Read1000StringEntries");

        winrt::hstring keyName = L"key";
        winrt::hstring value = L"my_value";
        for (int i = 0; i < 1000; ++i)
        {
            auto newKeyName = keyName + winrt::to_hstring(i);
            auto val = m_applicationData.LocalSettings().Values().TryLookup(newKeyName);
            if (val)
            {
                auto strVal = val.try_as<winrt::hstring>();
            }
        }

        debugLog.Stop();
        return debugLog.LogRecord();
    }

    void TestApplicationData::ReadAndWriteFromMultipleThreads()
	{
        if (!m_applicationData)
        {
            Init();
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