#pragma once

#include <ocidl.h>
#include <shobjidl_core.h>
#include <ShObjIdl.h>
#include <atlstr.h>
#include <iostream>

namespace winrt::AutoPlayApp
{
    static constexpr GUID AUTO_PLAY_HANDLER_GUID // {BAF2FA85-E121-4CC9-A841-CE335B6F917F}
    {
        0xBAF2FA85, 0xE121, 0x4CC9, { 0xA8, 0x41, 0xCE, 0x33, 0x5B, 0x6F, 0x91, 0x7F }
    };

	struct CoAutoPlayHandler : winrt::implements<CoAutoPlayHandler, IHWEventHandler>
	{
        // IHWEventHandler
        IFACEMETHODIMP Initialize(LPCWSTR pszParams);
        IFACEMETHODIMP HandleEvent(LPCWSTR pszDeviceID, LPCWSTR pszAltDeviceID, LPCWSTR pszEventType);
        IFACEMETHODIMP HandleEventWithContent(LPCWSTR pszDeviceID, LPCWSTR pszAltDeviceID, LPCWSTR pszEventType, LPCWSTR pszContentTypeHandler, IDataObject* pDataObject);
	};

    struct AutoPlayHandler_factory : winrt::implements<AutoPlayHandler_factory, IClassFactory>
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

            return winrt::make<CoAutoPlayHandler>()->QueryInterface(iid, result);
        }

        HRESULT __stdcall LockServer(BOOL) noexcept final
        {
            return S_OK;
        }
    };

    // ####### COM Helper types and functions

    class AutoPlayHandlerRegManager
    {
    public: 

        AutoPlayHandlerRegManager() = default;

        void register_autoPlayHandler()
        {
            DWORD registration{};

            winrt::check_hresult(::CoRegisterClassObject(
                AutoPlayApp::AUTO_PLAY_HANDLER_GUID,
                make<AutoPlayApp::AutoPlayHandler_factory>().get(),
                CLSCTX_LOCAL_SERVER,
                REGCLS_SINGLEUSE,
                &registration));
        }

        struct prop_variant : PROPVARIANT
        {
            prop_variant() noexcept : PROPVARIANT{}
            {
            }

            ~prop_variant() noexcept
            {
                clear();
            }

            void clear() noexcept
            {
                WINRT_VERIFY_(S_OK, ::PropVariantClear(this));
            }
        };

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

        void update_registry()
        {
            std::wstring key_path{ LR"(SOFTWARE\Classes\CLSID\{????????-????-????-????-????????????})" };
            ::StringFromGUID2(AutoPlayApp::AUTO_PLAY_HANDLER_GUID, key_path.data() + 23, 39);
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

            std::wcout << L"In " << key_path << L", registered local server at " << path << std::endl;
        }
    };
}