// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include <App.xaml.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;


namespace winrt::CsCppApp::implementation
{
    MainWindow::MainWindow()
    {
        InitializeComponent();
    }

    void MainWindow::btnInitializeDotNet_Click(Windows::Foundation::IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args)
    {
        GetXamlMetadataProvider();
    }

    void MainWindow::btnCsWinRtLauncher_Click(Windows::Foundation::IInspectable const& /*sender*/, Microsoft::UI::Xaml::RoutedEventArgs const& /*args*/)
    {
        OpenDialog();
    }

    void MainWindow::OpenDialog()
    {
        GetXamlMetadataProvider();

        CsWinRtComponent::Launcher::OpenDialogAsync(*this);
    }

    winrt::Microsoft::UI::Xaml::Markup::IXamlMetadataProvider MainWindow::GetXamlMetadataProvider()
    {
        if (m_metadataProvider == nullptr)
        {
            m_metadataProvider = winrt::CsWinRtComponent::CsWinRtComponent_XamlTypeInfo::XamlMetaDataProvider();

            auto currentApp = winrt::Microsoft::UI::Xaml::Application::Current().try_as<winrt::CsCppApp::implementation::App>();
            if (currentApp)
            {
                currentApp->AddOtherProvider(winrt::CsWinRtComponent::CsWinRtComponent_XamlTypeInfo::XamlMetaDataProvider());
            }
        }

        return m_metadataProvider;
    }
}
