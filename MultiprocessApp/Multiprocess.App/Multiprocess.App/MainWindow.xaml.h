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

        void btnLaunchService_Click(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);
        void btnLaunchServiceWithShell_Click(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);

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
