#pragma once

#include "MainWindow.g.h"

namespace Playground
{
    struct TestApplicationData;
    struct TestSaveApis;
}

namespace winrt::Playground::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow();

        void btnTestApplicationDataContainer_Click(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
        winrt::Windows::Foundation::IAsyncAction btnTestSaveDialogWin32Api_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        winrt::Windows::Foundation::IAsyncAction btnTestSaveDialogComShell_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    private:
        std::shared_ptr<::Playground::TestApplicationData> m_testApplicationData;
        std::shared_ptr<::Playground::TestSaveApis> m_testSaveApis;

        void UpdateTextBlock(winrt::hstring const& txt);
        HWND GetWindowHandle();
    };
}

namespace winrt::Playground::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
