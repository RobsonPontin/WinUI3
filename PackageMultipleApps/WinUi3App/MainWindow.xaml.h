// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "MainWindow.g.h"

#include "winrt/Microsoft.UI.Xaml.h"

namespace winrt::WinUi3App::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow();
        MainWindow(winrt::hstring const& filePath);

        winrt::fire_and_forget image_Loaded(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::WinUi3App::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
