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

    void MainWindow::btnComInterface_Click(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&)
    {
        /* This is a simple test to POC the creation of a new isntance using COM
         * Right now the COM object is cached and it guarantees the lifetime of the "child" process.
         * Once it is released, in order to create a new window, it will shutdown the previous process as well.
         */
        if (m_mySimpleComclass != nullptr)
        {
            m_mySimpleComclass.Release();
        }

        HRESULT hr = m_mySimpleComclass.CoCreateInstance(CLSID_COMServerAppSimpleClass, nullptr, CLSCTX_LOCAL_SERVER);
        if (SUCCEEDED(hr))
        {
            m_mySimpleComclass->Test();
        }
    }

    void MainWindow::btnComInterface2_Click(IInspectable const&, Microsoft::UI::Xaml::RoutedEventArgs const&)
    {
        HRESULT hr = m_myOtherSimpleComclass.CoCreateInstance(CLSID_MyOtherSimpleComClass, nullptr, CLSCTX_LOCAL_SERVER);
        if (SUCCEEDED(hr))
        {
            m_myOtherSimpleComclass->TestOther();
        }
    }
}
