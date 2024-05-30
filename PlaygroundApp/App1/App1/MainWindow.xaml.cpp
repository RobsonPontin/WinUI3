#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include "TestApplicationData.h"
#include "DebugLog.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::Playground::implementation
{
    MainWindow::MainWindow()
    {
        // Xaml objects should not call InitializeComponent during construction.
        // See https://github.com/microsoft/cppwinrt/tree/master/nuget#initializecomponent

        m_testApplicationData = std::make_shared<::Playground::TestApplicationData>();
    }

    void MainWindow::btnTestApplicationDataContainer_Click(IInspectable const&, RoutedEventArgs const&)
    {
        m_testApplicationData->Init();
        m_testApplicationData->Test();
    }
}
