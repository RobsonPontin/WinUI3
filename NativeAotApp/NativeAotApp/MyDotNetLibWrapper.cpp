#include "pch.h"
#include "MyDotNetLibWrapper.h"

#include <stdlib.h>
#include <stdio.h>
#include "windows.h"

#define symLoad GetProcAddress
#pragma comment (lib, "ole32.lib")

#ifndef F_OK
#define F_OK    0
#endif

namespace NativeAotApp::Wrappers
{
	const char* LIB_PATH = "MyDotNetLib.dll"; // it should be AppX root folder alongside App.exe

	MyDotNetLibWrapper::MyDotNetLibWrapper()
	{
	}

	bool MyDotNetLibWrapper::Initialize()
	{
		// Call sum function defined in C# shared library
		m_hInstance = LoadLibraryA(LIB_PATH);
		if (m_hInstance == NULL)
		{
			return false;
		}

		return true;
	}

	int MyDotNetLibWrapper::GetValue()
	{
		typedef int(*myFunc)();
		myFunc getValueImport = (myFunc)symLoad(m_hInstance, "getValue");
		if (getValueImport == NULL)
		{
			auto error = GetLastError();
			return -1;
		}

		return getValueImport();
	}

	winrt::hstring MyDotNetLibWrapper::GetName()
	{
		typedef char* (*myFunc)();
		myFunc getNameImport = (myFunc)symLoad(m_hInstance, "getName");
		if (getNameImport == NULL)
		{
			auto error = GetLastError();
			return L"";
		}

		return winrt::to_hstring(getNameImport());
	}

	int MyDotNetLibWrapper::FileExists(TCHAR* file)
	{
		WIN32_FIND_DATA FindFileData;
		HANDLE handle = FindFirstFile(file, &FindFileData);
		int found = handle != INVALID_HANDLE_VALUE;
		if (found)
		{
			//FindClose(&handle); this will crash
			FindClose(handle);
		}
		return found;
	}
}