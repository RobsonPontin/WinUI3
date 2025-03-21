#include "pch.h"

#include "FileReader.h"

#include <filesystem>
#include <fstream>

namespace Playground::Utils
{
	//  Forward Declare
	std::wstring ReadFileNameImpl(const std::wstring_view& filePath);
	void ReadFileNameAsyncImpl(std::promise<std::wstring>& promise, const std::wstring_view& filePath);

	void FileReader::Run()
	{
		while (true/*!stopToken.stop_requested()*/)
		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_conditionVariable.wait(lock, [this]
				{
					if (m_shutdownRequest)
						return true;

					return m_filePathRequest != L"";
				});

			if (m_shutdownRequest)
				break;

			// build the promise to be returned in the future
			std::promise<std::wstring> promise_fileName;
			std::future<std::wstring> future = promise_fileName.get_future();

			auto futureAsync = std::async(std::launch::async, &ReadFileNameAsyncImpl, std::ref(promise_fileName), std::move(m_filePathRequest));

			auto result = future.get();
			m_callbackOnFileNameReady(result);			
		}
	}

	FileReader::FileReader()
	{
		m_jThread = std::jthread(&FileReader::Run, this);
		m_shutdownRequest = false;
	}

	FileReader::~FileReader()
	{
		// Notify any threads waiting for notification to go ahead and process shutdown
		m_shutdownRequest = true;
		m_conditionVariable.notify_all();
	}

	std::wstring_view FileReader::ReadFileName(std::wstring_view file_path)
	{
		return ReadFileNameImpl(file_path);
	}

	void FileReader::ReadFileNameAsync(std::wstring_view file_path)
	{
		if (!std::filesystem::exists(file_path))
			return;

		m_filePathRequest = file_path;
		m_conditionVariable.notify_one();
	}

	void ReadFileNameAsyncImpl(std::promise<std::wstring>& promise, const std::wstring_view& filePath)
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
}