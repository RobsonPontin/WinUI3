#pragma once

#include "ComProxyData.g.h"

namespace winrt::IpcApp::implementation
{
	struct ComProxyData : ComProxyDataT<ComProxyData>
	{
		ComProxyData() = default;

		winrt::hstring Data()
		{
			return m_data;
		}

		void Data(winrt::hstring const& val)
		{
			m_data = val;
		}

	private:
		winrt::hstring m_data{ L"initial_val" };
	};
}

namespace winrt::IpcApp::factory_implementation
{
	struct ComProxyData : ComProxyDataT<ComProxyData, implementation::ComProxyData>
	{
	};
}
