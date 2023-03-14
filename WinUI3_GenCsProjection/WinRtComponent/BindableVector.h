// Copyright (c) Microsoft Corporation. All rights reserved.

#pragma once

#include "BindableVector.g.h"
#include "BindableIterator.g.h"

namespace winrt::WinRtComponent::implementation
{
    struct BindableVector : BindableVectorT<BindableVector>, winrt::vector_base<BindableVector, Windows::Foundation::IInspectable>
    {
        BindableVector();

        winrt::Microsoft::UI::Xaml::Interop::IBindableVectorView GetView();
        winrt::Microsoft::UI::Xaml::Interop::IBindableIterator First();

        auto& get_container() const noexcept
        {
            return m_items;
        }

        auto& get_container() noexcept
        {
            return m_items;
        }

    private:
        std::vector<Windows::Foundation::IInspectable> m_items;
    };
}

namespace winrt::WinRtComponent::factory_implementation
{
    struct BindableVector : BindableVectorT<BindableVector, implementation::BindableVector>
    {
    };
}
