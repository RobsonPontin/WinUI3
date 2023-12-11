#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include <atlbase.h>
#include <atlcom.h>

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::COMServerApp::implementation
{
    struct __declspec(uuid("ea7aaf40-5e06-426b-adeb-c5d423b0507f")) ICOMServerAppSimpleInterface : ::IUnknown
    {
        virtual HRESULT __stdcall StartApp() = 0;
    };

    class DECLSPEC_UUID("df2a6e21-9da3-4345-9d42-d234a548dad7")
        COMServerAppSimpleClass;

    void MainWindow::myButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        CComPtr<ICOMServerAppSimpleInterface> comServerInterface;
        HRESULT hr = comServerInterface.CoCreateInstance(__uuidof(COMServerAppSimpleClass));
        if (SUCCEEDED(hr))
        {
            comServerInterface->StartApp();
        }
    }
}
