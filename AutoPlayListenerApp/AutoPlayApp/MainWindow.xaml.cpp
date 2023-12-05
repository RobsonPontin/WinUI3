// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif
#include <iostream>
#include <string>
#include <Windows.h>

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
}
