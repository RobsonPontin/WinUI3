// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include "winrt/Windows.Storage.h"
#include <winrt/Windows.Storage.Streams.h>
#include "winrt/Microsoft.UI.Xaml.Media.Imaging.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::WinUi3App::implementation
{
    winrt::hstring m_fileActivationPath = L"";

    MainWindow::MainWindow()
    {
        InitializeComponent();
    }

    MainWindow::MainWindow(winrt::hstring const& filePath)
    {
        InitializeComponent();

        m_fileActivationPath = filePath;
    }

    winrt::fire_and_forget MainWindow::image_Loaded(IInspectable const&, RoutedEventArgs const&)
    {
        try
        {
            if (m_fileActivationPath == L"")
            {
                co_return;
            }

            auto file = co_await winrt::Windows::Storage::StorageFile::GetFileFromPathAsync(m_fileActivationPath);
            auto stream = co_await file.OpenAsync(Windows::Storage::FileAccessMode::Read);

            auto bmpImage = Microsoft::UI::Xaml::Media::Imaging::BitmapImage();
            co_await bmpImage.SetSourceAsync(stream);

            image().Source(bmpImage);
        }
        catch (winrt::hresult_error const&)
        {
            // error?
        }
    }
}