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

struct __declspec(uuid("ea7aaf40-5e06-426b-adeb-c5d423b0507f")) ICOMServerAppSimpleInterface : ::IUnknown
{
    virtual HRESULT __stdcall StartApp() = 0;
};

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
