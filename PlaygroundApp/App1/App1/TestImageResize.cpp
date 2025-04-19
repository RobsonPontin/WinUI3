#include "pch.h"
#include "TestImageResize.h"

#include <winrt/Windows.Storage.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Storage.FileProperties.h>
#include <winrt/Windows.Storage.Streams.h>

namespace Playground
{
    WF::IAsyncOperation<WGI::SoftwareBitmap> TestImageResize::ResizeImageAsync(WS::Streams::IRandomAccessStream imageStream, uint32_t targetSize)
    {
        try
        {
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
}