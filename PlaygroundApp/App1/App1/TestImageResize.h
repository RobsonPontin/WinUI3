#pragma once

#include "DebugLog.h"

namespace Playground
{
    struct TestImageResize
    {
        TestImageResize() = default;

        WF::IAsyncOperation<WGI::SoftwareBitmap> ResizeImageAsync(WS::Streams::IRandomAccessStream imageStream, uint32_t targetSize);
    };
}