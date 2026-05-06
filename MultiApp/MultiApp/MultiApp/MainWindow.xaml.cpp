#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include <string>

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::MultiApp::implementation
{
    void MainWindow::LaunchChildButton_Click(
        winrt::Windows::Foundation::IInspectable const& /*sender*/,
        winrt::Microsoft::UI::Xaml::RoutedEventArgs const& /*args*/)
    {
        wchar_t exePath[MAX_PATH]{};
        DWORD len = ::GetModuleFileNameW(nullptr, exePath, MAX_PATH);
        if (len == 0 || len == MAX_PATH)
        {
            StatusText().Text(L"Failed to resolve module path.");
            return;
        }

        std::wstring childPath{ exePath };
        size_t slash = childPath.find_last_of(L"\\/");
        if (slash != std::wstring::npos)
        {
            childPath.resize(slash + 1);
        }
        childPath += L"ChildApp.exe";

        // CreateProcessW requires a writable command-line buffer.
        std::wstring commandLine = L"\"" + childPath + L"\"";

        STARTUPINFOW si{};
        si.cb = sizeof(si);
        PROCESS_INFORMATION pi{};

        BOOL ok = ::CreateProcessW(
            childPath.c_str(),
            commandLine.data(),
            nullptr,
            nullptr,
            FALSE,
            0,
            nullptr,
            nullptr,
            &si,
            &pi);

        if (ok)
        {
            ::CloseHandle(pi.hThread);
            ::CloseHandle(pi.hProcess);
            StatusText().Text(L"Launched ChildApp (PID " + std::to_wstring(pi.dwProcessId) + L").");
        }
        else
        {
            DWORD err = ::GetLastError();
            StatusText().Text(L"CreateProcessW failed (error " + std::to_wstring(err) + L").");
        }
    }
}
