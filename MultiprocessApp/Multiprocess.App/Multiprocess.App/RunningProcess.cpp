#include "pch.h"

#include "RunningProcess.h"

#include <stdio.h>
#include <tchar.h>

namespace Multiprocess
{
    namespace Core
    {
        void RunningProcess::TryInitialize()
        {
            m_backgroundService = std::thread([=]()
                {
                    CreateBackgroundServiceImpl();
                });

            m_backgroundService.detach();
        }

        /* "The CreateProcess function creates a new process that runs independently of the creating process.
          * For simplicity, this relationship is called a parent-child relationship."
          * More Info: https://learn.microsoft.com/en-us/windows/win32/procthread/creating-processes */
        void RunningProcess::CreateBackgroundServiceImpl()
        {
            ZeroMemory(&m_srtartupInfo, sizeof(m_srtartupInfo));
            m_srtartupInfo.cb = sizeof(m_srtartupInfo);
            ZeroMemory(&m_processInfo, sizeof(m_processInfo));

            m_srtartupInfo.lpTitle = L"Background Service";

            TCHAR buffer[MAX_PATH] = { 0 };
            GetModuleFileName(NULL, buffer, MAX_PATH);
            std::wstring::size_type pos = std::wstring(buffer).find_last_of(L"\\/");
            auto path = std::wstring(buffer).substr(0, pos);

            path = path + L"\\Multiprocess.Service\\Multiprocess.Service.exe";

            // Start the child process. 
            auto hasSucceeded = CreateProcess(
                path.c_str(),     // Path to executable
                (LPWSTR)L"",      // Command line
                NULL,             // Process handle not inheritable
                NULL,             // Thread handle not inheritable
                FALSE,            // Set handle inheritance to FALSE
                0,                // No creation flags
                NULL,             // Use parent's environment block
                NULL,             // Use parent's starting directory 
                &m_srtartupInfo,  // Pointer to STARTUPINFO structure
                &m_processInfo);  // Pointer to PROCESS_INFORMATION structure

            if (!hasSucceeded)
            {
                printf("CreateProcess failed (%d).\n", GetLastError());
                return;
            }

            WaitForSingleObject(m_processInfo.hProcess, INFINITE);
            CloseHandle(m_processInfo.hProcess);
            CloseHandle(m_processInfo.hThread);
        }
    }
}