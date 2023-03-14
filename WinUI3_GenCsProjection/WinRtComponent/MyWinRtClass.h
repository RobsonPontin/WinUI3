// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "MyWinRtClass.g.h"

#include <winrt/Microsoft.UI.Xaml.Interop.h>

namespace winrt::WinRtComponent::implementation
{
    struct MyWinRtClass : MyWinRtClassT<MyWinRtClass>
    {
        MyWinRtClass() = default;

        int32_t MyLuckyNumber();
        void MyLuckyNumber(int32_t value);

        void TestCollection();

        winrt::event_token CollectionChanged(winrt::Microsoft::UI::Xaml::Interop::NotifyCollectionChangedEventHandler const& value);
        void CollectionChanged(winrt::event_token const& token);

    private:
        int32_t m_myLuckyNumber = 7;

        winrt::event<winrt::Microsoft::UI::Xaml::Interop::NotifyCollectionChangedEventHandler> m_collectionChanged;
    };
}

namespace winrt::WinRtComponent::factory_implementation
{
    struct MyWinRtClass : MyWinRtClassT<MyWinRtClass, implementation::MyWinRtClass>
    {
    };
}
