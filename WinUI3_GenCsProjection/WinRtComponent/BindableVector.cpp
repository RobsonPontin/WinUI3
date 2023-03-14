// Copyright (c) Microsoft Corporation. All rights reserved.

#include "pch.h"
#include "BindableVector.h"
#if __has_include("BindableVector.g.cpp")
#include "BindableVector.g.cpp"
#endif

namespace winrt::WinRtComponent::implementation
{
    BindableVector::BindableVector()
    {
    }

    winrt::Microsoft::UI::Xaml::Interop::IBindableVectorView BindableVector::GetView()
    {
        return *this;
    }

    winrt::Microsoft::UI::Xaml::Interop::IBindableIterator BindableVector::First()
    {
        return BindableIterator(*this);
    }
}
