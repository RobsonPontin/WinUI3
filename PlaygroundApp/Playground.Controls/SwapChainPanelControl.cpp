#include "pch.h"
#include "SwapChainPanelControl.h"
#if __has_include("SwapChainPanelControl.g.cpp")
#include "SwapChainPanelControl.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::Playground::Controls::implementation
{
    SwapChainPanelControl::SwapChainPanelControl()
    {
    }

    int32_t SwapChainPanelControl::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void SwapChainPanelControl::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }

    void SwapChainPanelControl::RootGrid_PointerMoved(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& args)
    {
        args.GetIntermediatePoints(Content());
    }
}
