// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.


#include <Unknwn.h>
#include <winrt/Windows.Foundation.h>

#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include "wil/com.h"
#include "winrt/impl/Windows.Foundation.1.h"

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;

namespace winrt::IpcApp::implementation
{


    MainWindow::MainWindow()
    {
        InitializeComponent();
    }

    int32_t MainWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    winrt::fire_and_forget MainWindow::btnMarshal_Click(IInspectable const&, RoutedEventArgs const&)
    {
		// Push this work onto a background thread.
		co_await resume_background();

		auto uninitOnExit = wil::CoInitializeEx();

		IpcApp::ComProxyServer comServer{};
		auto data = ComProxyData{};

		comServer.MarshalData(data);
    }

    winrt::fire_and_forget MainWindow::btnUnMarshal_Click(IInspectable const&, RoutedEventArgs const&)
    {
        // Push this work onto a background thread.
        co_await resume_background();

        auto uninitOnExit = wil::CoInitializeEx();

        IpcApp::ComProxyServer comServer{};

        comServer.UnmarshalData();
    }
}
