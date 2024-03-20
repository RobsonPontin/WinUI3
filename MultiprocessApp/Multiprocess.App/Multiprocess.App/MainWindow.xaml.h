#pragma once

#include "MainWindow.g.h"
#include "ProcessManager.h"

namespace winrt::Multiprocess::App::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow();

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void btnLaunchService_Click(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&);
        void btnLaunchServiceWithShell_Click(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&);
        void btnGetProcessesInfo_Click(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&);
        winrt::Windows::Foundation::IAsyncAction btnRedirectMain_Click(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&);
        winrt::Windows::Foundation::IAsyncAction btnRedirectSpare_Click(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&);

    private:
        std::shared_ptr<::Multiprocess::Core::ProcessManager> m_processManager{ nullptr };
    };
}

namespace winrt::Multiprocess::App::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
