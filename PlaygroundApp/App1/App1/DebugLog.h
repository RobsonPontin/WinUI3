#pragma once

namespace Playground
{
	struct DebugLog
	{
		DebugLog(std::wstring_view name);

		void Stop();

	private:
		struct DebugLogRecord
		{
			DebugLogRecord() = default;
			DebugLogRecord(std::wstring_view info, uint32_t totalElapsedTime);
			std::wstring Name;
			uint32_t TotalElapsedTime;
		};

		std::wstring m_name;
		std::chrono::steady_clock::time_point m_startTime;;

		DebugLogRecord m_record;
	};
}