#include "pch.h"
#include "UserControlCommon.xaml.h"
#if __has_include("UserControlCommon.g.cpp")
#include "UserControlCommon.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::LibXaml::Common::implementation
{
    int32_t UserControlCommon::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void UserControlCommon::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void UserControlCommon::myButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        myButton().Content(box_value(L"Clicked"));
    }
}
