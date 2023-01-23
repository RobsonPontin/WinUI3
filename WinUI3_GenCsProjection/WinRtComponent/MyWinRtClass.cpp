// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "MyWinRtClass.h"
#if __has_include("MyWinRtClass.g.cpp")
#include "MyWinRtClass.g.cpp"
#endif

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::WinRtComponent::implementation
{
    int32_t MyWinRtClass::MyLuckyNumber()
    {
        return m_myLuckyNumber;
    }

    void MyWinRtClass::MyLuckyNumber(int32_t value)
    {
        m_myLuckyNumber = value;
    }
}
