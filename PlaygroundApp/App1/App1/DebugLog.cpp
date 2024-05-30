#include "pch.h"
#include "DebugLog.h"

#include <chrono>

namespace Playground
{
	// Forward declaration
	void LogToOutput(const wchar_t* szFormat, ...);

	DebugLog::DebugLog(std::wstring_view name)
			: m_startTime(std::chrono::high_resolution_clock::now())
			, m_name(name)
		{
		}

	void DebugLog::Stop()
	{
		auto endTime = std::chrono::high_resolution_clock::now();
		uint32_t totalElapsedTime = static_cast<uint32_t>(std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_startTime).count());

		m_record = DebugLogRecord(m_name, totalElapsedTime);

		LogToOutput(L"[DebugLog][%ls] total %d ms \n", m_name.c_str(),m_record.TotalElapsedTime);
	}

	DebugLog::DebugLogRecord::DebugLogRecord(
		std::wstring_view name,
		uint32_t totalElapsedTime)
			: Name(name)
			, TotalElapsedTime(totalElapsedTime)
		{
		}


	void LogToOutput(const wchar_t* szFormat, ...)
	{
		wchar_t szBuff[1024];
		va_list arg;
		va_start(arg, szFormat);
		//_vsnprintf(szBuff, sizeof(szBuff), szFormat, arg);
		_vsnwprintf_s(szBuff, _TRUNCATE, szFormat, arg);
		va_end(arg);
		OutputDebugString(szBuff);
	}

}