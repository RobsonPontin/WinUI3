#pragma once

#include "CommunicationService.g.h"

namespace winrt::COM::ServerLib::implementation
{
    struct CommunicationService : CommunicationServiceT<CommunicationService>
    {
        CommunicationService() = default;

        int32_t MyProperty();
        void MyProperty(int32_t value);

        bool IsThisWorking() { return true; }

        Windows::Foundation::Collections::IVector<winrt::hstring> FilePaths() { return nullptr; }
    };
}

namespace winrt::COM::ServerLib::factory_implementation
{
    struct CommunicationService : CommunicationServiceT<CommunicationService, implementation::CommunicationService>
    {
    };
}
