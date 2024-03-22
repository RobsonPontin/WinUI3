#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include <atlbase.h>
#include <atlcom.h>

#include "COMServerAppSimpleClass.h"
#include "MyOtherSimpleComClass.h"

using namespace winrt;
using namespace winrt::Microsoft::UI::Xaml;

namespace winrt::COMServerApp::implementation
{
    CComPtr<IMySimpleComClass> m_mySimpleComclass;
    CComPtr<IMyOtherSimpleComClass> m_myOtherSimpleComclass;

    void MainWindow::myButton_Click(IInspectable const&, RoutedEventArgs const&)
    {
        HRESULT hr = m_mySimpleComclass.CoCreateInstance(CLSID_COMServerAppSimpleClass, nullptr, CLSCTX_LOCAL_SERVER);
        if (SUCCEEDED(hr))
        {
            m_mySimpleComclass->Test();
        }

        hr = m_myOtherSimpleComclass.CoCreateInstance(CLSID_MyOtherSimpleComClass, nullptr, CLSCTX_LOCAL_SERVER);
        if (SUCCEEDED(hr))
        {
            m_myOtherSimpleComclass->TestOther();
        }
    }
}
