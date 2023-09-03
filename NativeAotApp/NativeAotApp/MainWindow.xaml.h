// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "MainWindow.g.h"
#include "MyDotNetLibWrapper.h"

namespace winrt::NativeAotApp::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow();

        void myButton_Click(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args);

    private:
        std::unique_ptr<::NativeAotApp::Wrappers::MyDotNetLibWrapper> m_myDotNetLibWrapper{ nullptr };
    };
}

namespace winrt::NativeAotApp::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
