#pragma once

#include "DebugLog.h"

namespace Playground
{
    struct D2DImageResizer;
    struct WICImageResizer;

    struct TestImageResize
    {
        TestImageResize();

        WF::IAsyncOperation<WGI::SoftwareBitmap> ResizeImageWinRtAsync(WS::StorageFile file, uint32_t targetSize);
        WGI::SoftwareBitmap ResizeImageWIC(std::wstring_view const& path, uint32_t targetSize);
        // TODO: move d2d work to a background thread, but need a headless Window to pass the hardware target
        WF::IAsyncAction ResizeImageD2DAsync(HWND hWnd, std::wstring_view const& path);


    private:
        std::shared_ptr<D2DImageResizer> m_d2dImageResizer;
        std::shared_ptr< WICImageResizer> m_wicImageResizer;
    };
}