#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include <winrt/COM.ServerLib.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::COM::ClientApp::implementation
{
    int32_t MainWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    auto create_my_server_instance()
    {
        auto guid = winrt::guid("{96369f54-8eb6-48f0-abce-c1b211e627c3}");
        return winrt::create_instance<winrt::Windows::Foundation::IStringable>(guid, CLSCTX_LOCAL_SERVER);
    }

	winrt::Windows::Foundation::IStringable m_stringableInstance;

    void MainWindow::myButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        myButton().Content(box_value(L"Clicked"));

        if (m_stringableInstance == nullptr)
        {
            m_stringableInstance = create_my_server_instance();
        }

        auto value = m_stringableInstance.ToString();

        auto instance2 = create_my_server_instance();
        auto value2 = instance2.ToString();
    }

    void MainWindow::btnStop_Click(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args)
    {
        m_stringableInstance = nullptr;
    }

    void MainWindow::btnTestProxyStub_Click(IInspectable const& sender, Microsoft::UI::Xaml::RoutedEventArgs const& args)
    {
        auto guid = winrt::guid("{ec39b7db-b506-4489-804d-5844ec346b54}");
        auto instance = winrt::create_instance<winrt::COM::ServerLib::IProcessCommunication>(guid, CLSCTX_LOCAL_SERVER);
		auto result = instance.IsThisWorking();
        auto paths = instance.FilePaths();

        auto path = paths.First().Current();
    }
}
