// ComServerExe.cpp : Implementation of WinMain


#include "pch.h"
#include "framework.h"
#include "resource.h"
#include "ComServerExe_i.h"


using namespace ATL;


class CComServerExeModule : public ATL::CAtlExeModuleT< CComServerExeModule >
{
public :
	DECLARE_LIBID(LIBID_ComServerExeLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_COMSERVEREXE, "{8c755181-d4bb-4abe-8fee-e76f140e5dc6}")
};

CComServerExeModule _AtlModule;



//
extern "C" int WINAPI _tWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/,
								LPTSTR /*lpCmdLine*/, int nShowCmd)
{
	return _AtlModule.WinMain(nShowCmd);
}

