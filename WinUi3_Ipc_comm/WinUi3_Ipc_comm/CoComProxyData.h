#pragma once

#include <unknwn.h>

namespace IpcApp
{
    static constexpr GUID COM_PROXY_DATA_GUID // {BAF2FA77-E111-4Cb9-A841-CE335B6F917F}
    {
        0xBAF2FA77, 0xE111, 0x4Cb9, { 0xA8, 0x41, 0xCE, 0x33, 0x5B, 0x6F, 0x91, 0x7F }
    };

    struct __declspec(uuid("BAF2FA77-E111-4Cb9-A841-CE335B6F917F"))
		CoComProxyData : winrt::implements<CoComProxyData, IUnknown>
    {
        winrt::hstring ToString()
        {
            return L"MyCoclass as a string";
        }

	//	// Implement the IUnknown methods.
	//	ULONG STDMETHODCALLTYPE AddRef() noexcept override
	//	{ return InterlockedIncrement(&m_refCount); }
	//	ULONG STDMETHODCALLTYPE Release() noexcept override
	//	{ if (InterlockedDecrement(&m_refCount) == 0) { delete this; return 0; } return m_refCount; }

	//	HRESULT STDMETHODCALLTYPE QueryInterface(
	//		/* [in] */ REFIID riid,
	//		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject) noexcept override
	//	{
	//		if (riid == IID_IUnknown || riid == __uuidof(CoComProxyData))
	//		{
	//			*ppvObject = static_cast<CoComProxyData*>(this);
	//			AddRef();
	//			return S_OK;
	//		}
	//		*ppvObject = NULL;
	//		return E_NOINTERFACE;
	//	}

	//private:
	//	long m_refCount;
    };

    struct CoComProxyData_factory : winrt::implements<CoComProxyData_factory, IClassFactory>
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

            return winrt::make<CoComProxyData>()->QueryInterface(iid, result);
        }

        HRESULT __stdcall LockServer(BOOL) noexcept final
        {
            return S_OK;
        }
    };


	std::wstring get_module_path()
	{
		std::wstring path(100, L'?');
		uint32_t path_size{};
		DWORD actual_size{};

		do
		{
			path_size = static_cast<uint32_t>(path.size());
			actual_size = ::GetModuleFileName(nullptr, path.data(), path_size);

			if (actual_size + 1 > path_size)
			{
				path.resize(path_size * 2, L'?');
			}
		} while (actual_size + 1 > path_size);

		path.resize(actual_size);
		return path;
	}

	struct registry_traits
	{
		using type = HKEY;

		static void close(type value) noexcept
		{
			WINRT_VERIFY_(ERROR_SUCCESS, ::RegCloseKey(value));
		}

		static constexpr type invalid() noexcept
		{
			return nullptr;
		}
	};
	using registry_key = winrt::handle_type<registry_traits>;
	void UpdateRegistry(winrt::guid guid)
	{
		std::wstring key_path{ LR"(SOFTWARE\Classes\CLSID\{????????-????-????-????-????????????})" };
		::StringFromGUID2(guid, key_path.data() + 23, 39);
		key_path += LR"(\LocalServer32)";
		registry_key key;

		winrt::check_win32(::RegCreateKeyEx(
			HKEY_CURRENT_USER,
			key_path.c_str(),
			0,
			nullptr,
			0,
			KEY_WRITE,
			nullptr,
			key.put(),
			nullptr));
		::RegDeleteValue(key.get(), nullptr);

		std::wstring path{ get_module_path() };

		winrt::check_win32(::RegSetValueEx(
			key.get(),
			nullptr,
			0,
			REG_SZ,
			reinterpret_cast<BYTE const*>(path.c_str()),
			static_cast<uint32_t>((path.size() + 1) * sizeof(wchar_t))));
	}

	void Register()
	{
		DWORD registration{};

		winrt::check_hresult(::CoRegisterClassObject(
			::IpcApp::COM_PROXY_DATA_GUID,
			winrt::make<::IpcApp::CoComProxyData_factory>().get(),
			CLSCTX_LOCAL_SERVER,
			REGCLS_SINGLEUSE,
			&registration));
	}
}