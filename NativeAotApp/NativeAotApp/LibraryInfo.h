#pragma once

namespace NativeAotApp
{
	enum class DotNetType
	{
		Framework,
		Standard,
		Core
	};

	/// <summary>
	/// a matching type to deserialize to the Native AOT library type wrote in C#.
	/// </summary>
	class LibraryInfo
	{
	public:
		char* DotNetVersion;
		DotNetType DotNetType;
	};
}
