#include "pch.h"
#include "MyDotNetLibWrapper.h"

#include <stdlib.h>
#include <stdio.h>
#include "windows.h"

#define symLoad GetProcAddress
#pragma comment (lib, "ole32.lib")

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

	winrt::hstring MyDotNetLibWrapper::GetNameFromList()
	{
		typedef char* (*myFunc)();
		myFunc getNameImport = (myFunc)symLoad(m_hInstance, "getNameFromList");
		if (getNameImport == NULL)
		{
			auto error = GetLastError();
			return L"";
		}

		return winrt::to_hstring(getNameImport());
	}
}