#include "pch.h"

#include "AppLauncher.h"

#include <string>
#include <codecvt>
#include <vector>

namespace ComServerExe
{
	std::wstring utf8toUtf16(const std::string& str)
	{
		if (str.empty())
			return std::wstring();

		size_t charsNeeded = ::MultiByteToWideChar(CP_UTF8, 0,
			str.data(), (int)str.size(), NULL, 0);
		if (charsNeeded == 0)
			throw std::runtime_error("Failed converting UTF-8 string to UTF-16");

		std::vector<wchar_t> buffer(charsNeeded);
		int charsConverted = ::MultiByteToWideChar(CP_UTF8, 0,
			str.data(), (int)str.size(), &buffer[0], buffer.size());
		if (charsConverted == 0)
			throw std::runtime_error("Failed converting UTF-8 string to UTF-16");

		return std::wstring(&buffer[0], charsConverted);
	}

	HRESULT AppLauncher::StartWithParam(PCWSTR pszParams)
	{
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		// Get current path for directory where this module lives
		char buffer[MAX_PATH];
		auto result = GetModuleFileNameA(nullptr, buffer, MAX_PATH);

		if (result != 0)
		{
			// Convert to wstring and trim executable name
			std::wstring wStrThisModulePath = utf8toUtf16(buffer);
			std::size_t indexLastChartTilName = wStrThisModulePath.find_last_of(L"/\\");
			std::wstring wStrThisModulePathTrimmed = wStrThisModulePath.substr(0, indexLastChartTilName + 1);

			// The Console exe path from this directory
			std::wstring wStrConsoleExePathWithCmdArg = L"\\AutoPlayAppConsole\\AutoPlayAppConsole.exe -lp";

			// Construct the command line with target path + arguments
			std::wstring wStrParams(pszParams);
			std::wstring sStrCmdline = wStrThisModulePathTrimmed + wStrConsoleExePathWithCmdArg + L" " + wStrParams;

			// Start the child process.
			if (!CreateProcess(
				NULL,   // No module name (use command line)
				(LPWSTR)sStrCmdline.c_str(),        // Command line
				NULL,           // Process handle not inheritable
				NULL,           // Thread handle not inheritable
				FALSE,          // Set handle inheritance to FALSE
				0,              // No creation flags
				NULL,           // Use parent's environment block
				NULL,           // Use parent's starting directory
				&si,            // Pointer to STARTUPINFO structure
				&pi)           // Pointer to PROCESS_INFORMATION structure
				)
			{
				printf("CreateProcess failed (%d).\n", GetLastError());
				return S_FALSE;
			}

			// Wait until child process exits.
			WaitForSingleObject(pi.hProcess, INFINITE);

			// Close process and thread handles

			return S_OK;
		}

		return S_FALSE;
	}
};