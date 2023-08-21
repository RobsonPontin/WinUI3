#pragma once

#include "ComProxyServer.g.h"

#include "SharedFile.h"

namespace winrt::IpcApp::implementation
{
	// The layout of the memory being mapped.
	struct MappedMemoryValue
	{
		// The size of the memory block itself.
		static constexpr ULONG s_MemorySize = 4 << 10;

		HRESULT Result;
		ULONG FactorySize;
		uint8_t FactoryObject[1];

		// The maximum size of the marshalled object.
		static constexpr ULONG MaxFactorySize()
		{
			static_assert(s_MemorySize > offsetof(MappedMemoryValue, FactoryObject));
			return s_MemorySize - offsetof(MappedMemoryValue, FactoryObject);
		}
	};

	struct ComProxyServer : ComProxyServerT<ComProxyServer>
	{
	public:
		ComProxyServer();

		void MarshalData(winrt::IpcApp::ComProxyData const& data);
		void UnmarshalData();

	private:
		::IpcApp::SharedMemory<uint8_t> m_data;
	};
}

namespace winrt::IpcApp::factory_implementation
{
	struct ComProxyServer : ComProxyServerT<ComProxyServer, implementation::ComProxyServer>
	{
	};
}
