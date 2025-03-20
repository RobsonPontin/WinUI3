#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include <iostream>

#include <winrt/Microsoft.UI.Xaml.Documents.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Microsoft.UI.Windowing.h>
#include <microsoft.ui.xaml.window.h>

#include <../../Playground.Utils/TaskRunner.h>
#include <../../Playground.Utils/TestTaskBlockThread.h>

#include "TestApplicationData.h"
#include "TestLauncher.h"
#include "TestPickerApis.h"

#include "DebugLog.h"

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;

namespace winrt::Playground::implementation
{
    const winrt::hstring APP_ASSETS_IMAGE_JPG_TEST{ L"ms-appx:///Assets/image.jpg" };

    MainWindow::MainWindow()
    {
        // Xaml objects should not call InitializeComponent during construction.
        // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent

        m_testApplicationData = std::make_shared<::Playground::TestApplicationData>();
        m_testSaveApis = std::make_shared<::Playground::TestPickerApis>();
        m_testLauncher = std::make_shared<::Playground::TestLauncher>();
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
        m_testLauncher->StartWithShellApi();
    }

    void MainWindow::btnLaunchProcessWin32CreateProcessApi_Click(IInspectable const&, RoutedEventArgs const&)
    {
        m_testLauncher->StartWithWin32CreateProcessApi();
    }

    void MainWindow::btnLaunchProcessActivationManager_Click(IInspectable const&, RoutedEventArgs const&)
    {
        m_testLauncher->StartWithActivationManager();
    }

    void MainWindow::btnLaunchProcessActivationManagerForFile_Click(IInspectable const&,RoutedEventArgs const&)
    {
        m_testLauncher->StartWithActivationManagerForFile(APP_ASSETS_IMAGE_JPG_TEST);
    }

    void MainWindow::btnLaunchPhotosAppWithProtocolLaunchForFile_Click(IInspectable const&, RoutedEventArgs const&)
    {
        // NOTE: change file path to match your case
        winrt::hstring filePath = L"C:\\Users\\robsonpontin\\Pictures\\image.jpg";
        m_testLauncher->StartPhotosAppWithProtocolLaunchForFile(filePath);
    }

    void MainWindow::btnToggleExtendsContentIntoTitleBar_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
    {
        if (this->ExtendsContentIntoTitleBar())
        {
            this->ExtendsContentIntoTitleBar(false);
        }
        else
        {
            this->ExtendsContentIntoTitleBar(true);
        }
    }

    bool TestFuncCallback()
    {
        std::cout << "Task - Starting delay..." << std::endl;

        // TODO: after sleep, it will resume to a different background thread
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Task - delay finished." << std::endl;

        return true;
    }

    void MainWindow::btnTestDatastructures_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
    {
        if (m_taskQueueRunner == nullptr)
        {
            m_taskQueueRunner = std::make_shared<::Playground::Utils::TaskRunner>();
            m_taskQueueRunner->Run();
        }

        m_taskQueueRunner->AddTask(&TestFuncCallback);

        auto taskTestBlockThread = std::make_shared<::Playground::Utils::TestTaskBlockThread>(::Playground::Utils::TaskPriority::High);
        auto taskTestBlockThread2 = std::make_shared<::Playground::Utils::TestTaskBlockThread>(::Playground::Utils::TaskPriority::Low);
        auto taskTestBlockThread3 = std::make_shared<::Playground::Utils::TestTaskBlockThread>(::Playground::Utils::TaskPriority::Low);
        auto taskTestBlockThread4 = std::make_shared<::Playground::Utils::TestTaskBlockThread>(::Playground::Utils::TaskPriority::Low);
        auto taskTestBlockThread5 = std::make_shared<::Playground::Utils::TestTaskBlockThread>(::Playground::Utils::TaskPriority::High);
        auto taskTestBlockThread6 = std::make_shared<::Playground::Utils::TestTaskBlockThread>(::Playground::Utils::TaskPriority::High);
        m_taskQueueRunner->AddTask(taskTestBlockThread);
        m_taskQueueRunner->AddTask(taskTestBlockThread2);
        m_taskQueueRunner->AddTask(taskTestBlockThread3);
        m_taskQueueRunner->AddTask(taskTestBlockThread4);
        m_taskQueueRunner->AddTask(taskTestBlockThread5);
        m_taskQueueRunner->AddTask(taskTestBlockThread6);

    }
}
