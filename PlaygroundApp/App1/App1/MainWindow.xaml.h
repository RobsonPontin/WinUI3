#pragma once

#include "MainWindow.g.h"

namespace Playground
{
    struct TestApplicationData;
}

namespace winrt::Playground::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow();

        void btnTestApplicationDataContainer_Click(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);

    private:
        std::shared_ptr<::Playground::TestApplicationData> m_testApplicationData;
    };
}

namespace winrt::Playground::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
