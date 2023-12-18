// AutoPlayHandler.h : Declaration of the CAutoPlayHandler

#pragma once
#include "resource.h"       // main symbols

#include "ComServerExe_i.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

#include <thread>

#include "AppLauncher.h"

using namespace ATL;


// CAutoPlayHandler

class ATL_NO_VTABLE CAutoPlayHandler :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CAutoPlayHandler, &CLSID_AutoPlayHandler>,
	// public IDispatchImpl<IAutoPlayHandler, &IID_IAutoPlayHandler, &LIBID_ComServerExeLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
	public IDropTarget,
	public IHWEventHandler2
{
public:
	CAutoPlayHandler()
	{
	}

DECLARE_REGISTRY_RESOURCEID(106)


BEGIN_COM_MAP(CAutoPlayHandler)
	COM_INTERFACE_ENTRY(IDropTarget)
	COM_INTERFACE_ENTRY(IHWEventHandler)
	COM_INTERFACE_ENTRY(IHWEventHandler2)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

	// IDropTarget
	STDMETHODIMP DragEnter(
		_In_        IDataObject* pDataObj,
		_In_        DWORD grfKeyState,
		_In_        POINTL pt,
		_Out_       DWORD* pdwEffect)
	{
		UNREFERENCED_PARAMETER(pDataObj);
		UNREFERENCED_PARAMETER(grfKeyState);
		UNREFERENCED_PARAMETER(pt);

		*pdwEffect = DROPEFFECT_COPY;

		return S_OK;
	}

	STDMETHODIMP DragOver(
		_In_        DWORD grfKeyState,
		_In_        POINTL pt,
		_Out_       DWORD* pdwEffect)
	{
		UNREFERENCED_PARAMETER(grfKeyState);
		UNREFERENCED_PARAMETER(pt);

		*pdwEffect = DROPEFFECT_COPY;

		return S_OK;
	}

	STDMETHODIMP DragLeave()
	{
		return S_OK;
	}

	STDMETHODIMP Drop(
		_In_        IDataObject* pDataObj,
		_In_        DWORD grfKeyState,
		_In_        POINTL pt,
		_Out_       DWORD* pdwEffect)
	{
		UNREFERENCED_PARAMETER(pDataObj);
		UNREFERENCED_PARAMETER(grfKeyState);
		UNREFERENCED_PARAMETER(pt);

		*pdwEffect = DROPEFFECT_COPY;

		return S_OK;
	}

	// IHWEventHandler
	STDMETHODIMP Initialize(
		_In_ PCWSTR pszParams)
	{
		UNREFERENCED_PARAMETER(pszParams);

		return S_OK;
	}

	STDMETHODIMP HandleEvent(
		_In_ PCWSTR pszDeviceID,
		_In_ PCWSTR pszAltDeviceID,
		_In_ PCWSTR pszEventType)
	{
		UNREFERENCED_PARAMETER(pszAltDeviceID);

		return S_OK;
	}

	STDMETHODIMP HandleEventWithContent(
		_In_ PCWSTR pszDeviceID,
		_In_ PCWSTR pszAltDeviceID,
		_In_ PCWSTR pszEventType,
		_In_ PCWSTR pszContentTypeHandler,
		_In_ IDataObject* pDataObject)
	{
		UNREFERENCED_PARAMETER(pszDeviceID);
		UNREFERENCED_PARAMETER(pszAltDeviceID);
		UNREFERENCED_PARAMETER(pszEventType);
		UNREFERENCED_PARAMETER(pszContentTypeHandler);
		UNREFERENCED_PARAMETER(pDataObject);

		return S_OK;
	}

	// IHWEventHandler2
	STDMETHODIMP HandleEventWithHWND(
		_In_ PCWSTR pszDeviceID,
		_In_ PCWSTR pszAltDeviceID,
		_In_ PCWSTR pszEventType,
		_In_ HWND hwndOwner)
	{
		UNREFERENCED_PARAMETER(pszAltDeviceID);
		UNREFERENCED_PARAMETER(pszEventType);
		UNREFERENCED_PARAMETER(hwndOwner);

		/* ATTENTION:
		 * 
		 * Handlers that implement this interface should return quickly from calls to 
		 * IHWEventHandler2::HandleEventWithHWND so they won't block the AutoPlay dialog from closing.
		 * 
		 * For this reason, we create a new thread to handle the work of launching the app.
		 */

		std::thread thread([=]()
			{
				::ComServerExe::AppLauncher::StartWithParam(pszDeviceID);
			});

		thread.detach();

		return S_OK;
	}
};

OBJECT_ENTRY_AUTO(__uuidof(AutoPlayHandler), CAutoPlayHandler)
