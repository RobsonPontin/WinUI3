#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

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

    void MainWindow::myButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        myButton().Content(box_value(L"Clicked"));

		auto instance = create_my_server_instance();
        auto value = instance.ToString();

        auto instance2 = create_my_server_instance();
        auto value2 = instance.ToString();
    }
}
