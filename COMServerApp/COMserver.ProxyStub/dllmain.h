// dllmain.h : Declaration of module class.

class CCOMserverProxyStubModule : public ATL::CAtlDllModuleT< CCOMserverProxyStubModule >
{
public :
	DECLARE_LIBID(LIBID_COMserverProxyStubLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_COMSERVERPROXYSTUB, "{3316872a-c95e-4d36-a8f9-5fbd6c04d441}")
};

extern class CCOMserverProxyStubModule _AtlModule;
