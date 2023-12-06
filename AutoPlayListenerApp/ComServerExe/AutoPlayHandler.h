// AutoPlayHandler.h : Declaration of the CAutoPlayHandler

#pragma once
#include "resource.h"       // main symbols

#include "ComServerExe_i.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

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
		LaunchApp();
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

	HRESULT LaunchApp()
	{
		// Launch Photos ??
		STARTUPINFO si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		// Replace this string with the command line you want to execute
		TCHAR cmdLine[] = _T("D:\\Github\\WinUI3\\AutoPlayListenerApp\\AutoPlayAppPackage.WAP\\bin\\x64\\Debug\\AutoPlayAppConsole\\AutoPlayAppConsole.exe -lp");

		// Start the child process.
		if (!CreateProcess(NULL,   // No module name (use command line)
			cmdLine,        // Command line
			NULL,           // Process handle not inheritable
			NULL,           // Thread handle not inheritable
			FALSE,          // Set handle inheritance to FALSE
			0,              // No creation flags
			NULL,           // Use parent's environment block
			NULL,           // Use parent's starting directory
			&si,            // Pointer to STARTUPINFO structure
			&pi)           // Pointer to PROCESS_INFORMATION structure
			)
		{
			printf("CreateProcess failed (%d).\n", GetLastError());
			return S_FALSE;
		}

		// Wait until child process exits.
		WaitForSingleObject(pi.hProcess, INFINITE);

		// Close process and thread handles

		return S_OK;
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
		UNREFERENCED_PARAMETER(hwndOwner);

		return S_OK;
	}
};

OBJECT_ENTRY_AUTO(__uuidof(AutoPlayHandler), CAutoPlayHandler)
