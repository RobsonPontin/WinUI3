// MyOtherSimpleComClass.h : Declaration of the CMyOtherSimpleComClass

#pragma once
#include "resource.h"       // main symbols



#include "COMserverProxyStub_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;


// CMyOtherSimpleComClass

class ATL_NO_VTABLE CMyOtherSimpleComClass :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMyOtherSimpleComClass, &CLSID_MyOtherSimpleComClass>,
	public IUnknown,
	public IDispatchImpl<IMyOtherSimpleComClass, &IID_IMyOtherSimpleComClass, &LIBID_COMserverProxyStubLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CMyOtherSimpleComClass()
	{
	}

DECLARE_REGISTRY_RESOURCEID(107)


BEGIN_COM_MAP(CMyOtherSimpleComClass)
	COM_INTERFACE_ENTRY(IMyOtherSimpleComClass)
	COM_INTERFACE_ENTRY(IUnknown)
END_COM_MAP()

	HRESULT TestOther()
	{
		return S_OK;
	}

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:



};

OBJECT_ENTRY_AUTO(__uuidof(MyOtherSimpleComClass), CMyOtherSimpleComClass)
