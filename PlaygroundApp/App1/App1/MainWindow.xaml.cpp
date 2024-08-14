#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include <winrt/Microsoft.UI.Xaml.Documents.h>
#include <winrt/Windows.Storage.h>
#include <microsoft.ui.xaml.window.h>

#include <filesystem>

#include "TestApplicationData.h"
#include "TestPickerApis.h"

#include "DebugLog.h"

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;

namespace winrt::Playground::implementation
{
    MainWindow::MainWindow()
    {
        // Xaml objects should not call InitializeComponent during construction.
        // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent

        m_testApplicationData = std::make_shared<::Playground::TestApplicationData>();
        m_testSaveApis = std::make_shared<::Playground::TestPickerApis>();
    }

    void MainWindow::btnTestApplicationDataContainer_Click(IInspectable const&, RoutedEventArgs const&)
    {
        auto log = m_testApplicationData->Init();
        UpdateTextBlock(L"Init ApplicationData::Current() " + winrt::to_hstring(log.TotalElapsedTime) + L" ms");

        auto log2 = m_testApplicationData->Write1000StringEntries();
        UpdateTextBlock(L"Write 1000 strings " + winrt::to_hstring(log2.TotalElapsedTime) + L" ms");

        auto log3 = m_testApplicationData->Read1000StringEntries();
        UpdateTextBlock(L"Read 1000 strings " + winrt::to_hstring(log3.TotalElapsedTime) + L" ms");
    }

    void MainWindow::UpdateTextBlock(winrt::hstring const& txt)
    {
        winrt::Microsoft::UI::Xaml::Documents::Paragraph txtParagraph;
        winrt::Microsoft::UI::Xaml::Documents::Run txtRun;

        txtRun.Text(txt);
        txtParagraph.Inlines().Append(txtRun);

        btnRtbFeedback().Blocks().Append(txtParagraph);
    }

    winrt::Windows::Foundation::IAsyncAction MainWindow::btnTestSaveDialogWin32Api_Click(IInspectable const&, RoutedEventArgs const&)
    {
        auto hWnd = GetWindowHandle();        
        auto file = co_await m_testSaveApis->OpenFilePickerWinRTAsync(hWnd);
        if (file != nullptr)
        {
            auto str = winrt::to_string(file.Path());
            m_testSaveApis->OpenSaveFileDialogWin32(hWnd, str);
        }
    }

    winrt::Windows::Foundation::IAsyncAction MainWindow::btnTestSaveDialogComShell_Click(IInspectable const&, RoutedEventArgs const&)
    {
        co_await m_testSaveApis->OpenSaveFileDialogComShellAsync();
    }

    HWND MainWindow::GetWindowHandle()
    {
        auto windowNative{ (*this).try_as<::IWindowNative>() };
        winrt::check_bool(windowNative);

        HWND hWnd{ 0 };
        windowNative->get_WindowHandle(&hWnd);

        return hWnd;
    }

    void MainWindow::btnLaunchProcessFromShellApi_Click(IInspectable const&, RoutedEventArgs const&)
    {
        // This is defined in the AppxManifest
        std::wstring filePath = L"PlaygroundApp.exe";

        SHELLEXECUTEINFO sei{};
        sei.cbSize = sizeof(SHELLEXECUTEINFO);
        sei.fMask = SEE_MASK_DEFAULT;
        sei.lpVerb = L"open";
        sei.lpFile = filePath.data();
        auto quotedFilePath = std::format(L"\"{}\"", filePath.data());
        sei.lpParameters = quotedFilePath.c_str();
        sei.nShow = SW_SHOWNORMAL;
        if (!ShellExecuteEx(&sei))
        {
            // Failed.
        }
    }
}
