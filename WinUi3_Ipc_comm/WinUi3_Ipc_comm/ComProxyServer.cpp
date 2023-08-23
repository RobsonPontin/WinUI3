#include "pch.h"

#include "ComProxyServer.h"
#if __has_include("ComProxyServer.g.cpp")
#include "ComProxyServer.g.cpp"
#endif

#include "wil/win32_helpers.h"
#include "wil/resource.h"
#include "CoComProxyData.h"

namespace winrt::IpcApp::implementation
{
	static PCWSTR REQUEST_PACKET_NAME_FORMAT = L"_ComProxyServer";
	static PCWSTR PROCESS_NAME = L"_IpcApp";

	static constexpr GUID server_clsid // DAA16D7F-EF66-4FC9-B6F2-3E5B6D924576
	{
		0xdaa16d7f, 0xef66, 0x4fc9, { 0xb6, 0xf2, 0x3e, 0x5b, 0x6d, 0x92, 0x45, 0x76 }
	};

	struct Stringable : implements<Stringable, winrt::Windows::Foundation::IStringable>
	{
		hstring ToString()
		{
			return L"Hello from server";
		}
	};

	struct Factory : implements<Factory, IClassFactory>
	{
		HRESULT __stdcall CreateInstance(
			IUnknown* outer,
			GUID const& iid,
			void** result) noexcept final
		{
			*result = nullptr;

			if (outer)
			{
				return CLASS_E_NOAGGREGATION;
			}

			return make<Stringable>().as(iid, result);
		}

		HRESULT __stdcall LockServer(BOOL) noexcept final
		{
			return S_OK;
		}
	};


	ComProxyServer::ComProxyServer()
	{
		GUID id;
		THROW_IF_FAILED(CoCreateGuid(&id));

		wil::unique_cotaskmem_string idString;
		THROW_IF_FAILED(StringFromCLSID(id, &idString));

		m_data.Open(idString.get());
	}

	void ComProxyServer::MarshalData(winrt::IpcApp::ComProxyData const& data)
	{
		ULONG streamSize{ 0 };
		std::wstring uri;

#ifndef USE_COM_CLASS
		const auto uuidofArgs = ::IpcApp::COM_PROXY_DATA_GUID;

		::IpcApp::Register();
		::IpcApp::UpdateRegistry(uuidofArgs);
		
		auto server = create_instance<::IpcApp::CoComProxyData>(uuidofArgs, CLSCTX_LOCAL_SERVER);
		::IpcApp::CoComProxyData* pMyInterface = NULL;
		HRESULT hr2 = server->QueryInterface(__uuidof(::IpcApp::CoComProxyData), (void**)&pMyInterface);
		if (SUCCEEDED(hr2))
		{
			pMyInterface->Release();
		}
#else
		winrt::com_ptr<::IUnknown> unk{ data.as<::IUnknown>() };
		const auto uuidofArgs = guid_of<IpcApp::IComProxyData>();
#endif

		com_ptr<IStream> stream;

		HRESULT hr = CoGetMarshalSizeMax(
			&streamSize, 
			uuidofArgs, 
			unk.get(), 
			MSHCTX_LOCAL, 
			nullptr, 
			MSHLFLAGS_NORMAL);

		if (FAILED(hr))
			return;

		// Add space for the marshaling type data and resize the backing storage.
		m_data.Resize(sizeof(bool) + streamSize);

		uint8_t* streamStart = (m_data.Get() + sizeof(bool));

		hr = CreateStreamOnHGlobal(
			nullptr,
			TRUE,
			stream.put());

		if (FAILED(hr))
			return;

		hr = CoMarshalInterface(
			stream.get(),
			uuidofArgs,
			unk.get(),
			MSHCTX_LOCAL,
			nullptr,
			MSHLFLAGS_NORMAL);

		if (FAILED(hr))
		{
			stream->Release();
			return;
		}

		const LARGE_INTEGER headOffset{};
		auto resetStreamOnExit = wil::scope_exit([&stream, &headOffset]
			{
				stream->Seek(headOffset, STREAM_SEEK_SET, nullptr);
				CoReleaseMarshalData(stream.get());
			});

		stream->Seek(headOffset, STREAM_SEEK_SET, nullptr);

		STATSTG stats{};
		THROW_IF_FAILED(stream->Stat(&stats, STATFLAG_NONAME));
		THROW_HR_IF(E_FAIL, static_cast<ULONG>(stats.cbSize.QuadPart) > streamSize);

		ULONG bytesRead{ 0 };
		THROW_IF_FAILED(stream->Read(streamStart, static_cast<ULONG>(stats.cbSize.QuadPart), &bytesRead));

		//resetStreamOnExit.release();

		stream->Seek(headOffset, STREAM_SEEK_SET, nullptr);

		// Marshal an HRESULT into the stream.
		HRESULT hrToMarshal = E_FAIL;
		CoMarshalHresult(stream.get(), hrToMarshal);

		com_ptr<::IUnknown> unk2;
		hr = CoUnmarshalInterface(stream.get(), uuidofArgs, unk2.put_void());

		auto err = GetLastError();
	}

	void ComProxyServer::UnmarshalData()
	{
		// The first byte holds data about the marshaling type to use.
		uint8_t* streamStart = (m_data.Get() + sizeof(bool));
		ULONG streamSize = (static_cast<ULONG>(m_data.Size()) - sizeof(bool));

		// Use COM stream marshaling.
		winrt::com_ptr<IStream> stream;
		HRESULT hr = CreateStreamOnHGlobal(nullptr, TRUE, stream.put());
		if (FAILED(hr))
		{
			return;
		}

		ULONG bytesWritten = 0;
		THROW_IF_FAILED(stream->Write(streamStart, streamSize, &bytesWritten));

		const LARGE_INTEGER headOffset{};
		THROW_IF_FAILED(stream->Seek(headOffset, STREAM_SEEK_SET, nullptr));

		const auto uuidofArgs = guid_of<IpcApp::ComProxyData>();
		com_ptr<::IUnknown> unk;
		hr = CoUnmarshalInterface(stream.get(), uuidofArgs, unk.put_void());
		if (FAILED(hr))
		{
			stream->Release();
			return;
		}

		auto data = unk.as<IpcApp::ComProxyData>();
	}
}