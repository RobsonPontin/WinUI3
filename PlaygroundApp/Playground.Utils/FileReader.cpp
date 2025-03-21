#include "pch.h"

#include "FileReader.h"

#include <filesystem>
#include <fstream>

namespace Playground::Utils
{
	// Forward declare
	std::wstring ReadFileNameImpl(const std::wstring_view& filePath);

	FileReader::FileReader()
	{
		m_jThread = std::jthread(&FileReader::Run, this);
		m_jthread2 = std::jthread(&FileReader::Run2, this);

		m_shutdownRequest = false;
	}

	FileReader::~FileReader()
	{
		// Notify any threads waiting for notification to go ahead and process shutdown
		m_shutdownRequest = true;
		m_conditionVariable.notify_all();
		m_conditionVariable2.notify_all();
	}

	std::wstring_view FileReader::ReadFileName(std::wstring_view file_path)
	{
		auto result = ReadFileNameImpl(file_path);
		return result;
	}

	void FileReader::ReadFileNameAsync(std::wstring_view file_path)
	{
		if (!std::filesystem::exists(file_path))
			return;

		std::lock_guard<std::mutex> guard(m_mutex);
		m_filesQueue.push(std::wstring(file_path));
		m_conditionVariable.notify_one();
	}

	void FileReader::ReadFileNameAsync2(std::wstring_view file_path)
	{
		if (!std::filesystem::exists(file_path))
			return;

		std::lock_guard<std::mutex> guard(m_mutex2);
		m_filesQueue2.push(std::wstring(file_path));
		m_conditionVariable2.notify_one();
	}

	std::wstring ReadFileNameWithAsyncFuture(const std::wstring_view& filePath)
	{
		auto result = ReadFileNameImpl(filePath);
		return result;
	}

	void ReadFileNameWithPromiseFuture(std::promise<std::wstring>& promise, const std::wstring_view& filePath)
	{
		auto result = ReadFileNameImpl(filePath);
		if (result == L"")
		{
			promise.set_exception(std::make_exception_ptr(std::runtime_error("Failed to obtain file name.")));
		}

		promise.set_value(result);
	}

	std::wstring ReadFileNameImpl(const std::wstring_view& filePath)
	{
		std::wstring path{ filePath.data(), filePath.size() };
		std::wifstream file(path);
		if (file.is_open())
		{
			// simulate heavy work
			std::this_thread::sleep_for(std::chrono::seconds(2));

			std::wstring property;
			std::getline(file, property);
			file.close();

			return property;
		}

		return L"";
	}

	void FileReader::Run()
	{
		while (true)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_conditionVariable.wait(lock, [this]
				{
					if (m_shutdownRequest)
						return true;

					return !m_filesQueue.empty();
				});

			if (m_shutdownRequest)
				break;

			auto filePath = m_filesQueue.front();
			m_filesQueue.pop();

			auto futureAsync = std::async(std::launch::async, &ReadFileNameWithAsyncFuture, std::move(filePath));

			auto result = futureAsync.get();
			m_callbackOnFileNameReady(result);
		}
	}

	void FileReader::Run2()
	{
		while (true)
		{
			std::unique_lock<std::mutex> lock(m_mutex2);
			m_conditionVariable2.wait(lock, [this]
				{
					if (m_shutdownRequest)
						return true;

					return !m_filesQueue2.empty();
				});

			if (m_shutdownRequest)
				break;

			auto filePath = m_filesQueue2.front();
			m_filesQueue2.pop();

			std::promise<std::wstring> promise;
			std::future<std::wstring> future = promise.get_future();

			// std::ref(promise) -> when starting a thread it will copy its arguments but std::promise is not copyable, so it passes by reference
			std::jthread worker(&ReadFileNameWithPromiseFuture, std::ref(promise), filePath);

			auto result = future.get();
			m_callbackOnFileNameReady(result);
		}
	}
}