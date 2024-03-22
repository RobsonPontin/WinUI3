#pragma once

// Needed to be included before any cppwinrt to support classic COM interfaces.
#include <unknwn.h>

#include <wil/resource.h>
#include <winrt/Windows.Foundation.h>
#include "LifetimeManager.h"


struct __declspec(uuid("4eb7afe3-94bb-480a-9e7e-ad9d788cf04a")) IMyOtherSimpleComClass : public ::IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE TestOther(void) = 0;
};

// "df2a6e21-9da3-4345-9d42-d234a548dad7"
static const GUID CLSID_MyOtherSimpleComClass =
{ 0xdf2a6e21, 0x9da3, 0x4145, { 0x9d, 0x42, 0xd2, 0x32, 0xa5, 0x48, 0xda, 0xd7 } };

struct __declspec(uuid("df2a6e21-9da3-4145-9d42-d232a548dad7")) MyOtherSimpleComClass : winrt::implements<MyOtherSimpleComClass,
    IPersist,
    IMyOtherSimpleComClass>
{
public:
    MyOtherSimpleComClass()
    {
        COMServerApp::LifetimeManager::CreateShared()->Lock();
    }

    ~MyOtherSimpleComClass()
    {
        COMServerApp::LifetimeManager::CreateShared()->Unlock();
    }

    HRESULT __stdcall TestOther()
    {
        return S_OK;
    }

    HRESULT __stdcall GetClassID(CLSID* id) noexcept override
    {
        *id = IID_IPersist; // Doesn't matter what we return, for this example.
        return S_OK;
    }

    winrt::hstring ToString()
    {
        return L"MyOtherSimpleComClass as a string";
    }
};