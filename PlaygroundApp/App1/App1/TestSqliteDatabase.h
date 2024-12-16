#pragma once

#include "DebugLog.h"

namespace Playground
{
	struct TestSqliteDatabase
	{
		TestSqliteDatabase() = default;

		bool Connect();

	private:
		std::unique_ptr<sqlite3> m_db;
	};
}
