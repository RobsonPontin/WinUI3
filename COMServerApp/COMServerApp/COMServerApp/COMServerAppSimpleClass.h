#pragma once

// Needed to be included before any cppwinrt to support classic COM interfaces.
#include <unknwn.h>

#include <wil/resource.h>
#include <winrt/Windows.Foundation.h>
#include "LifetimeManager.h"


// This GUID is the same GUID that was provided in the 
// registration of the COM Server and Class Id in the .appxmanifest.
struct __declspec(uuid("3c63c457-55af-45db-b66c-6bc9822c94f5")) IMySimpleComClass : public ::IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE Test(void) = 0;
};

// "df2a6e21-9da3-4345-9d42-d234a548dad7"
static const GUID CLSID_COMServerAppSimpleClass =
{ 0xdf2a6e21, 0x9da3, 0x4345, { 0x9d, 0x42, 0xd2, 0x34, 0xa5, 0x48, 0xda, 0xd7 } };

struct __declspec(uuid("df2a6e21-9da3-4345-9d42-d234a548dad7")) COMServerAppSimpleClass : winrt::implements<COMServerAppSimpleClass,
    IPersist,
    IMySimpleComClass>
{
public:
    COMServerAppSimpleClass()
    {
        COMServerApp::LifetimeManager::CreateShared()->Lock();
    }

    ~COMServerAppSimpleClass()
    {
        COMServerApp::LifetimeManager::CreateShared()->Unlock();
    }

    HRESULT __stdcall Test()
    {
        // kicks off a main thread
        std::thread thread([=]()
            {
                // call DLL main func
                wWinMain(0, 0, L"", 0);
            });

        thread.detach();

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
