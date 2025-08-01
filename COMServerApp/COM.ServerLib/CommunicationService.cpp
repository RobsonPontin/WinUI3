#include "pch.h"
#include "CommunicationService.h"
#if __has_include("CommunicationService.g.cpp")
#include "CommunicationService.g.cpp"
#endif

namespace winrt::COM::ServerLib::implementation
{
    int32_t CommunicationService::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void CommunicationService::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }
}
