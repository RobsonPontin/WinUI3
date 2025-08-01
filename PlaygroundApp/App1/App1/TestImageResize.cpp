#include "pch.h"
#include "TestImageResize.h"

#include <iostream>
#include <filesystem>

#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Storage.FileProperties.h>
#include <winrt/Windows.Storage.Streams.h>

#include "D2DImageResizer.h"
#include "WICImageResizer.h"
#include "ImageView.h"

namespace Playground
{
    TestImageResize::TestImageResize()
    {
        m_d2dImageResizer = std::make_shared<D2DImageResizer>();
        m_wicImageResizer = std::make_shared<WICImageResizer>();
    }

    WF::IAsyncOperation<WGI::SoftwareBitmap> TestImageResize::ResizeImageWinRtAsync(WS::StorageFile file, uint32_t targetSize)
    {
        try
        {
            auto imageStream = co_await file.OpenAsync(WS::FileAccessMode::Read);
            auto decoder = co_await WGI::BitmapDecoder::CreateAsync(imageStream);
            WGI::IBitmapFrameWithSoftwareBitmap bitmapFrameWithSoftwareBitmap;
            decoder.as(bitmapFrameWithSoftwareBitmap);

            WGI::BitmapTransform bitmapTransform;
            bitmapTransform.ScaledWidth(targetSize);
            bitmapTransform.ScaledHeight(targetSize);

            // NOTE: Fant seems to give the best looking image after resize, with smoother look.
            // NearestNeighbor and Bicubic are pretty close in quality with a "Pixelated look"
            WG::Imaging::BitmapInterpolationMode interoplationMode = WG::Imaging::BitmapInterpolationMode::Fant;
            bitmapTransform.InterpolationMode(interoplationMode);

            auto softwareBitmap = co_await bitmapFrameWithSoftwareBitmap.GetSoftwareBitmapAsync(
                WGI::BitmapPixelFormat::Bgra8,
                WGI::BitmapAlphaMode::Premultiplied, // Note: for xaml render with BitmapSoftwareSource only supports "pre-multiplied or no alpha."
                bitmapTransform,
                WGI::ExifOrientationMode::IgnoreExifOrientation,
                WGI::ColorManagementMode::ColorManageToSRgb);

            auto numPixelsPerChannel = softwareBitmap.PixelWidth() * softwareBitmap.PixelHeight();

            WSS::Buffer buffer = WSS::Buffer(numPixelsPerChannel * 4);
            softwareBitmap.CopyToBuffer(buffer);

            co_return softwareBitmap;
        }
        catch (winrt::hresult const&)
        {
            // winrt error
        }
        catch (...)
        {
            // Unknown error
        }

        co_return nullptr;
    }
    
    WGI::SoftwareBitmap TestImageResize::ResizeImageWIC(std::wstring_view const& path, uint32_t targetSize)
    {
        auto imageResizedBuffer = m_wicImageResizer->DecodeAndScaleImage(path, targetSize);
        if (!imageResizedBuffer.has_value())
        {
            return nullptr;
        }

        auto softwareBitmap = WGI::SoftwareBitmap::CreateCopyFromBuffer(
            imageResizedBuffer.value(),
            WGI::BitmapPixelFormat::Bgra8, 
            targetSize, 
            targetSize,
            WGI::BitmapAlphaMode::Ignore);

        return softwareBitmap;
    }

    WF::IAsyncAction TestImageResize::ResizeImageD2DAsync(HWND hWnd, std::wstring_view const& path)
    {
        auto result = m_d2dImageResizer->TryInitialize(hWnd);
        if (!result)
        {
            co_return;
        }

        m_d2dImageResizer->ScaleImage(path);

        co_return;
    }
}