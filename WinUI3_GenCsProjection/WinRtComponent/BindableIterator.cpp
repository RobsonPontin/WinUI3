// Copyright (c) Microsoft Corporation. All rights reserved.

#include "pch.h"
#include "BindableIterator.h"
#if __has_include("BindableIterator.g.cpp")
#include "BindableIterator.g.cpp"
#endif

namespace winrt::WinRtComponent::implementation
{
    BindableIterator::BindableIterator(winrt::Microsoft::UI::Xaml::Interop::IBindableVector const& bindableVector)
        : m_bindableVector(bindableVector)
    {
        m_current = 0;
    }

    Windows::Foundation::IInspectable BindableIterator::Current()
    {
        if (HasCurrent())
        {
            return m_bindableVector.GetAt(m_current);
        }
        else
        {
            return nullptr;
        }
    }

    bool BindableIterator::HasCurrent()
    {
        return (m_current >= 0 && static_cast<uint32_t>(m_current) < m_bindableVector.Size());
    }

    bool BindableIterator::MoveNext()
    {
        m_current++;
        return HasCurrent();
    }
}
