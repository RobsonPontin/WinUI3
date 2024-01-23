#include "pch.h"
#include "UserControl.xaml.h"
#if __has_include("UserControl.g.cpp")
#include "UserControl.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::LibXaml::implementation
{
    int32_t UserControl::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void UserControl::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void UserControl::myButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        myButton().Content(box_value(L"Clicked"));
    }
}
