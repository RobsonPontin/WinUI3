#pragma once

namespace Playground
{
	struct DebugLogRecord
	{
		DebugLogRecord() = default;
		DebugLogRecord(std::wstring_view info, uint32_t totalElapsedTime);

		std::wstring Name;
		uint32_t TotalElapsedTime;
	};

	struct DebugLog
	{
		DebugLog(std::wstring_view name);

		void Stop();

		DebugLogRecord LogRecord() { return m_record; }

	private:

		std::wstring m_name;
		std::chrono::steady_clock::time_point m_startTime;;

		DebugLogRecord m_record;
	};
}