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

    WF::IAsyncOperation<WGI::SoftwareBitmap> TestImageResize::ResizeImageWinRtAsync(WS::Streams::IRandomAccessStream imageStream, uint32_t targetSize)
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
    
    WF::IAsyncOperation<WGI::SoftwareBitmap> TestImageResize::ResizeImageWICAsync(std::wstring_view const& path, uint32_t targetSize)
    {
        auto result = m_wicImageResizer->TryInitialize();
        if (!result)
        {
            co_return nullptr;
        }

        ImageView imageView;

        // TODO: temp, testing decoding from WIC directly and no winrt needed
        bool useWinrtDecoding = true;

        if (useWinrtDecoding)
        {
            auto file = co_await WS::StorageFile::GetFileFromPathAsync(path);
            auto stream = co_await file.OpenAsync(WS::FileAccessMode::Read);
            auto decoder = co_await WGI::BitmapDecoder::CreateAsync(stream);
			auto pixelData = co_await decoder.GetPixelDataAsync();
			auto pixels = pixelData.DetachPixelData();

            std::vector<uint8_t> pixelsVector{ pixels.begin(), pixels.end() };

            imageView = ImageView(
				decoder.OrientedPixelWidth(),
				decoder.OrientedPixelHeight(),
                pixelsVector,
                GUID_WICPixelFormat32bppBGRA);
        }
        else
        {
            auto result = m_wicImageResizer->DecodeImage(path);
            if (!result.has_value())
            {
                co_return nullptr;
            }

            imageView = result.value();
        }

        auto imageResizedBuffer = m_wicImageResizer->ScaleImage(imageView, targetSize);
        if (imageResizedBuffer == nullptr)
        {
            co_return nullptr;
        }

        auto softwareBitmap = WGI::SoftwareBitmap::CreateCopyFromBuffer(
            imageResizedBuffer, 
            WGI::BitmapPixelFormat::Bgra8, 
            targetSize, 
            targetSize,
            WGI::BitmapAlphaMode::Ignore);

        co_return softwareBitmap;
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