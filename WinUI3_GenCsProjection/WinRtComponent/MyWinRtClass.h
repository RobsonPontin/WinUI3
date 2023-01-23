// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "MyWinRtClass.g.h"

namespace winrt::WinRtComponent::implementation
{
    struct MyWinRtClass : MyWinRtClassT<MyWinRtClass>
    {
        MyWinRtClass() = default;

        int32_t MyLuckyNumber();
        void MyLuckyNumber(int32_t value);

    private:
        int32_t m_myLuckyNumber = 7;
    };
}

namespace winrt::WinRtComponent::factory_implementation
{
    struct MyWinRtClass : MyWinRtClassT<MyWinRtClass, implementation::MyWinRtClass>
    {
    };
}
