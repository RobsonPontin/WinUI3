// Copyright (c) Microsoft Corporation. All rights reserved.

#pragma once

#include "BindableIterator.g.h"

namespace winrt::WinRtComponent::implementation
{
    struct BindableIterator : BindableIteratorT<BindableIterator>
    {
        BindableIterator(winrt::Microsoft::UI::Xaml::Interop::IBindableVector const& bindableVector);

        Windows::Foundation::IInspectable Current();
        bool HasCurrent();
        bool MoveNext();
    private:
        int m_current;
        winrt::Microsoft::UI::Xaml::Interop::IBindableVector m_bindableVector;
    };
}

namespace winrt::WinRtComponent::factory_implementation
{
    struct BindableIterator : BindableIteratorT<BindableIterator, implementation::BindableIterator>
    {
    };
}
