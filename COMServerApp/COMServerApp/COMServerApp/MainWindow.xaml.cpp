#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include <atlbase.h>
#include <atlcom.h>

#include "COMServerAppSimpleClass.h"

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;

namespace winrt::COMServerApp::implementation
{
    void MainWindow::myButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        // TODO: implement ICOMServerAppSimpleInterface once proxy dll is set
        CComPtr<IMySimpleComClass> comServerInterface;

        // TODO: this call will fail due to the App:wWinMain() that will return "0" right away.
        HRESULT hr = comServerInterface.CoCreateInstance(CLSID_COMServerAppSimpleClass, nullptr, CLSCTX_LOCAL_SERVER);
        if (SUCCEEDED(hr))
        {
            comServerInterface->Test();
        }
    }
}
