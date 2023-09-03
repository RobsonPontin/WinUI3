// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::NativeAotApp::implementation
{
    MainWindow::MainWindow()
    {
        InitializeComponent();

        m_myDotNetLibWrapper = std::make_unique<::NativeAotApp::Wrappers::MyDotNetLibWrapper>();
    }

    void MainWindow::myButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        if (m_myDotNetLibWrapper->Initialize())
        {
            auto val = m_myDotNetLibWrapper->GetValue();
            auto name = m_myDotNetLibWrapper->GetName();
            auto nameFromList = m_myDotNetLibWrapper->GetNameFromList();
        }        
    }
}
