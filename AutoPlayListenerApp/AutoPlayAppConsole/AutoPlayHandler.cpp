#include "pch.h"
#include "AutoPlayHandler.h"


namespace winrt::AutoPlayApp
{
    // ###################### IHWEventHandler

    IFACEMETHODIMP CoAutoPlayHandler::Initialize(LPCWSTR /*pszParams*/)
    {
        return S_OK;
    }

    IFACEMETHODIMP CoAutoPlayHandler::HandleEvent(LPCWSTR /*pszDeviceID*/, LPCWSTR /*pszAltDeviceID*/, LPCWSTR /*pszEventType*/)
    {
        //  Not Implemented
        return S_OK;
    }

    IFACEMETHODIMP CoAutoPlayHandler::HandleEventWithContent(LPCWSTR /*pszDeviceID*/, LPCWSTR /*pszAltDeviceID*/, LPCWSTR /*pszEventType*/, LPCWSTR /*pszContentTypeHandler*/, IDataObject* /*pDataObject*/)
    {
        //  Not Implemented
        return S_OK;
    }
}