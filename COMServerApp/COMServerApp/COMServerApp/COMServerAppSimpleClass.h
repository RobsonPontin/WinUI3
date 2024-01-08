#pragma once

// Needed to be included before any cppwinrt to support classic COM interfaces.
#include <unknwn.h>

#include <wil/resource.h>
#include <winrt/Windows.Foundation.h>


// This GUID is the same GUID that was provided in the 
// registration of the COM Server and Class Id in the .appxmanifest.
static constexpr GUID IDD_COMServer_App_Simple_Interface_clsid
{ /* ea7aaf40-5e06-426b-adeb-c5d423b0507f */
    0xea7aaf40, 0x5e06, 0x426b, {0xad, 0xeb, 0xc5, 0xd4, 0x23, 0xb0, 0x50, 0x7f}
};

// TODO: COM interfaces need a proxy stub dll to be consumed.
struct __declspec(uuid("ea7aaf40-5e06-426b-adeb-c5d423b0507f")) ICOMServerAppSimpleInterface : ::IUnknown
{
    virtual HRESULT __stdcall StartApp() = 0;
};

// "df2a6e21-9da3-4345-9d42-d234a548dad7"
static const GUID CLSID_COMServerAppSimpleClass =
{ 0xdf2a6e21, 0x9da3, 0x4345, { 0x9d, 0x42, 0xd2, 0x34, 0xa5, 0x48, 0xda, 0xd7 } };

struct __declspec(uuid("df2a6e21-9da3-4345-9d42-d234a548dad7")) COMServerAppSimpleClass : winrt::implements<COMServerAppSimpleClass,
    IPersist,
    ICOMServerAppSimpleInterface>
{
public:
    COMServerAppSimpleClass()
    {
    }

    HRESULT __stdcall StartApp() noexcept override
    {
        // kicks off a main thread
        std::thread([=]()
            {
                // call DLL main func
                wWinMain(0, 0, 0, 0);
            });

        return S_OK;
    }

    HRESULT __stdcall GetClassID(CLSID* id) noexcept override
    {
        *id = IID_IPersist; // Doesn't matter what we return, for this example.
        return S_OK;
    }

    winrt::hstring ToString()
    {
        return L"COMServerAppSimpleClass as a string";
    }
};

struct COMServerAppSimpleClass_Factory : winrt::implements<COMServerAppSimpleClass_Factory, IClassFactory>
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

        return winrt::make<COMServerAppSimpleClass>()->QueryInterface(iid, result);
    }

    HRESULT __stdcall LockServer(BOOL) noexcept final
    {
        return S_OK;
    }
};
