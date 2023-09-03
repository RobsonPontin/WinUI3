#pragma once


namespace NativeAotApp::Wrappers
{
	/// <summary>
	/// A wrapper around loading and accessing a .NET library published as AOT.
	/// </summary>
	struct MyDotNetLibWrapper : std::enable_shared_from_this<MyDotNetLibWrapper>
	{
		MyDotNetLibWrapper();

		int GetValue();
		winrt::hstring GetName();
		winrt::hstring GetNameFromList();

		bool Initialize();

	private:
		HINSTANCE m_hInstance;
	};
}
