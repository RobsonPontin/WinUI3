#pragma once

#include "SwapChainPanelControl.g.h"

namespace winrt::Playground::Controls::implementation
{
    struct SwapChainPanelControl : SwapChainPanelControlT<SwapChainPanelControl>
    {
        SwapChainPanelControl();

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void RootGrid_PointerMoved(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& args);
    };
}

namespace winrt::Playground::Controls::factory_implementation
{
    struct SwapChainPanelControl : SwapChainPanelControlT<SwapChainPanelControl, implementation::SwapChainPanelControl>
    {
    };
}
