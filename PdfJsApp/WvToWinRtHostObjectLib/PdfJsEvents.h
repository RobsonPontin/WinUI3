// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "PdfJsEvents.g.h"

namespace winrt::WvToWinRtHostObjectLib::implementation
{
    struct PdfJsEvents : PdfJsEventsT<PdfJsEvents>
    {
        PdfJsEvents() = default;

        winrt::hstring LastCopiedText() 
        {
            return m_lastCopiedText;
        }

        void LastCopiedText(winrt::hstring const& val)
        {
            m_lastCopiedText = val;
        }

        void RunTestCall()
        {
            OutputDebugStringW(L"[PdfJsEvents] RunTestCall()");
        }

    private:
        winrt::hstring m_lastCopiedText{ L"" };
    };
}

namespace winrt::WvToWinRtHostObjectLib::factory_implementation
{
    struct PdfJsEvents : PdfJsEventsT<PdfJsEvents, implementation::PdfJsEvents>
    {
    };
}
