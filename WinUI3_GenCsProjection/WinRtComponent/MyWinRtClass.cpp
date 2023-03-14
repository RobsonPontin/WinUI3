// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "MyWinRtClass.h"
#if __has_include("MyWinRtClass.g.cpp")
#include "MyWinRtClass.g.cpp"
#endif


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

	void MyWinRtClass::TestCollection()
	{
		WinRtComponent::BindableVector newVector{};
		winrt::hstring stringNewVal = L"new value";
			
		newVector.Append(winrt::box_value(stringNewVal));
		newVector.Append(winrt::box_value(stringNewVal));
		newVector.Append(winrt::box_value(stringNewVal));

		WinRtComponent::BindableVector oldVector{};
		winrt::hstring stringoldVal = L"old value";

		oldVector.Append(winrt::box_value(stringoldVal));
		oldVector.Append(winrt::box_value(stringoldVal));
		oldVector.Append(winrt::box_value(stringoldVal));

		m_collectionChanged(*this, 
			winrt::Microsoft::UI::Xaml::Interop::NotifyCollectionChangedEventArgs(
				winrt::Microsoft::UI::Xaml::Interop::NotifyCollectionChangedAction::Add,
				newVector,
				oldVector,
				0,
				1));
	}

	winrt::event_token MyWinRtClass::CollectionChanged(winrt::Microsoft::UI::Xaml::Interop::NotifyCollectionChangedEventHandler const& handler)
	{
		return m_collectionChanged.add(handler);
	}

	void MyWinRtClass::CollectionChanged(winrt::event_token const& token)
	{
		m_collectionChanged.remove(token);
	}
}
