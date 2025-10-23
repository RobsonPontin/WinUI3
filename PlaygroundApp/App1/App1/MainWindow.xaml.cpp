#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include <winrt/Microsoft.UI.Xaml.Documents.h>
#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Storage.FileProperties.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Microsoft.UI.Windowing.h>
#include <microsoft.ui.xaml.window.h>
#include <winrt/Microsoft.UI.Xaml.Media.Imaging.h>

#include "TestApplicationData.h"
#include "TestLauncher.h"
#include "TestImageResize.h"
#include "TestPickerApis.h"
#include "TestVideoApis.h"

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
        m_testImageResize = std::make_shared<::Playground::TestImageResize>();
		m_testVideoApis = std::make_shared<::Playground::TestMediaPlayerApis>();

        // Events registration
        m_testVideoApis->FrameImageReady({ get_weak(), &MainWindow::TestVideoApis_VideoFrameReady});
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

    WF::IAsyncAction MainWindow::btnTestVideoApis_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
    {
        auto file = co_await m_testSaveApis->OpenFilePickerForVideoWinRTAsync(GetWindowHandle());
        if (!file)
        {
            co_return;
        }

        co_await m_testVideoApis->LoadVideoFileAsync(file);
    }

    WF::IAsyncAction MainWindow::btnTestVideoExtractFrameApis_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        auto file = co_await m_testSaveApis->OpenFilePickerForVideoWinRTAsync(GetWindowHandle());
        if (!file)
        {
            co_return;
        }

        try
        {
			WF::TimeSpan playbackPosition = std::chrono::seconds(5);
            auto frameStreamResult = m_testVideoApis->ExtractFrameFromVideoAsync(file, playbackPosition);
            if (!frameStreamResult)
            {
                co_return;
            }
        }
        catch (...)
        {
            // fail
        }
    }

    WF::IAsyncAction MainWindow::btnTestVideoExtractFrameV2Apis_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        auto file = co_await m_testSaveApis->OpenFilePickerForVideoWinRTAsync(GetWindowHandle());
        if (!file)
        {
            co_return;
        }

        try
        {
			WF::TimeSpan playbackPosition = std::chrono::seconds(1); // TODO: for testing just choose second 1 of video
            m_testVideoApis->RequestFrameFromVideoAsync(file, playbackPosition);
        }
        catch (...)
        {
            // fail
        }
    }

    WF::IAsyncAction MainWindow::TestVideoApis_VideoFrameReady(WGI::SoftwareBitmap const& bitmap)
    {
        WUX::Media::Imaging::SoftwareBitmapSource imgSource;
        auto width = bitmap.PixelWidth();
        auto height = bitmap.PixelHeight();
        co_await imgSource.SetBitmapAsync(bitmap);

        ImageControl().Source(imgSource);
        ImageControl().Width(width);
        ImageControl().Height(height);
    }

    WF::IAsyncAction MainWindow::btnResizeImageTest_Click(winrt::Windows::Foundation::IInspectable const&, winrt::Microsoft::UI::Xaml::RoutedEventArgs const&)
    {
        auto file = co_await m_testSaveApis->OpenFilePickerWinRTAsync(GetWindowHandle());
        if (!file)
        {
            co_return;
        }

        try
        {
            uint32_t targetSize = 256;
            auto imgResult = co_await m_testImageResize->ResizeImageWinRtAsync(file, targetSize);
            if (!imgResult)
            {
                co_return;
            }            

            // NOTE: SoftwareBitmapSource::SetBitmapAsync only supports bgra8 pixel format and pre-multiplied or no alpha.
            WUX::Media::Imaging::SoftwareBitmapSource imgSource;
            co_await imgSource.SetBitmapAsync(imgResult);

            ImageControl().Source(imgSource);
            ImageControl().Width(targetSize);
            ImageControl().Height(targetSize);
        }
        catch (...)
        {
            // fail
        }
    }

    WF::IAsyncAction MainWindow::btnResizeImageWICTest_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        auto file = co_await m_testSaveApis->OpenFilePickerWinRTAsync(GetWindowHandle());
        if (!file)
        {
            co_return;
        }

        try
        {
            uint32_t targetSize = 256;
            auto imgResult = m_testImageResize->ResizeImageWIC(file.Path(), targetSize);
            if (!imgResult)
            {
                co_return;
            }

            // NOTE: SoftwareBitmapSource::SetBitmapAsync only supports bgra8 pixel format and pre-multiplied or no alpha.'
            WUX::Media::Imaging::SoftwareBitmapSource imgSource;
            co_await imgSource.SetBitmapAsync(imgResult);

            ImageControl().Source(imgSource);
            ImageControl().Width(targetSize);
            ImageControl().Height(targetSize);
        }
        catch (...)
        {
            // fail
        }
    }
}
