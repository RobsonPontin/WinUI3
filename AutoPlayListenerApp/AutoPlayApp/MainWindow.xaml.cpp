// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include <atlbase.h>
#include <atlcom.h>

#include <iostream>
#include <string>
#include <Windows.h>
#include <tchar.h>
#include "shobjidl.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// A list of all known devices that have been connected to PC.
#define AUTOPLAY_KNOWN_DEVICES_SUBKEY L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\AutoplayHandlers\\KnownDevices"

namespace winrt::AutoPlayApp::implementation
{
    MainWindow::MainWindow()
    {
        InitializeComponent();

        this->Title(L"AutoPlay listener App");
;    }

    int32_t MainWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void MainWindow::myButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        HKEY hKey = nullptr;
        DWORD retCode = ERROR_SUCCESS;

        auto result = RegOpenKeyEx(
            HKEY_CURRENT_USER,
            AUTOPLAY_KNOWN_DEVICES_SUBKEY,
            0,
            KEY_READ,
            &hKey);

        if (result != ERROR_SUCCESS)
        {
            return;
        }

        std::vector<std::wstring> kownDevices;
        DWORD i = 0;

        while (retCode == ERROR_SUCCESS)
        {
            wchar_t subKeyName[MAX_PATH];
            DWORD cbName = MAX_PATH;
            retCode = RegEnumKeyEx(
                hKey,
                i,
                subKeyName,
                &cbName,
                nullptr,
                nullptr,
                nullptr,
                nullptr);
            if (retCode == ERROR_SUCCESS)
            {
                kownDevices.emplace_back(subKeyName);
            }
            ++i;
        }
    }


    // COM Server Interface
    class DECLSPEC_UUID("a5402ef8-0beb-427d-ab14-2231593c419c")
    AutoPlayHandler : public IHWEventHandler
    {
    public:

    };

    void winrt::AutoPlayApp::implementation::MainWindow::btnTestComServer_Click(IInspectable const&, RoutedEventArgs const&)
    {
        CComPtr<IHWEventHandler> atlSimpleObject;
        HRESULT hr = atlSimpleObject.CoCreateInstance(__uuidof(AutoPlayHandler));
        if (SUCCEEDED(hr))
        {
            auto hr = atlSimpleObject->Initialize(0);
            if (SUCCEEDED(hr))
            {
                printf("Success!");
            }
        }

        atlSimpleObject.Release();
    }

    winrt::Windows::Foundation::IAsyncAction winrt::AutoPlayApp::implementation::MainWindow::btnTestCmdLine_Click(IInspectable const&, RoutedEventArgs const&)
    {
        co_await winrt::resume_background();

        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        // Replace this string with the command line you want to execute
        TCHAR cmdLine[] = _T("D:\\Github\\WinUI3\\AutoPlayListenerApp\\AutoPlayAppPackage.WAP\\bin\\x64\\Debug\\AutoPlayAppConsole\\AutoPlayAppConsole.exe -lp");

        // Start the child process.
        if (!CreateProcess(NULL,   // No module name (use command line)
            cmdLine,        // Command line
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
            co_return;
        }

        // Wait until child process exits.
        WaitForSingleObject(pi.hProcess, INFINITE);

        // Close process and thread handles
    }
}
