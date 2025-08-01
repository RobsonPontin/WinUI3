#pragma once

#include <wincodec.h>
#include <thumbcache.h>
#include <ShObjIdl_core.h>

#include "ImageView.h"

namespace Playground
{
    /// <summary>
    /// Scale image using WIC.
    /// Doc: https://learn.microsoft.com/en-us/windows/win32/wic/-wic-bitmapsources-howto-scale
    /// </summary>
    struct WICImageResizer
    {
        WICImageResizer()
        {
        }

        bool TryInitialize()
        {
            if (m_isInitialized)
            {
                return true;
            }

            HRESULT hr = CoCreateInstance(
                CLSID_WICImagingFactory2,
                nullptr,
                CLSCTX_INPROC_SERVER,
                IID_PPV_ARGS(&m_iWICImagingFactory));


            if (SUCCEEDED(hr))
            {
                m_isInitialized = true;
            }

            return m_isInitialized;
        }

        std::optional<WSS::IBuffer> DecodeAndScaleImage(std::wstring_view path, int32_t newSize)
        {
            if (!TryInitialize())
            {
                return std::nullopt;
            }

            // Step 1: Decode the source image
            winrt::com_ptr<IWICBitmapDecoder> pDecoder = nullptr;

            std::wstring pathCpy(path);
            HRESULT hr = m_iWICImagingFactory->CreateDecoderFromFilename(
                pathCpy.c_str(),                 // Image to be decoded
                nullptr,                         // Do not prefer a particular vendor
                GENERIC_READ,                    // Desired read access to the file
                WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
                pDecoder.put());                 // Pointer to the decoder

            if (FAILED(hr))
            {
                return std::nullopt;
            }

            // Step 2: Retrieve the first frame of the image from the decoder

            winrt::com_ptr<IWICBitmapFrameDecode> pFrame = nullptr;
            hr = pDecoder->GetFrame(0, pFrame.put());

            if (FAILED(hr))
            {
                return std::nullopt;
            }

            // Step 3: Format convert the frame to 32bppRGBA
            winrt::com_ptr<IWICFormatConverter> pConverter = nullptr;
            hr = m_iWICImagingFactory->CreateFormatConverter(pConverter.put());

            if (FAILED(hr))
            {
                return std::nullopt;
            }

            WICPixelFormatGUID pixelFormatGUID = GUID_WICPixelFormat32bppBGRA;

            hr = pConverter->Initialize(
                pFrame.get(),                   // Input bitmap to convert
                pixelFormatGUID,                // Destination pixel format
                WICBitmapDitherTypeNone,        // Specified dither pattern
                NULL,                           // Specify a particular palette 
                0.0f,                           // Alpha threshold
                WICBitmapPaletteTypeCustom);    // Palette translation type

            if (FAILED(hr))
            {
                return std::nullopt;
            }

            // Step 4: Scale image to target size
            IWICBitmapScaler* scaler;

            hr = m_iWICImagingFactory->CreateBitmapScaler(&scaler);

            if (FAILED(hr))
            {
                return nullptr;
            }

            hr = scaler->Initialize(
                pConverter.get(),
                newSize,
                newSize,
                WICBitmapInterpolationMode::WICBitmapInterpolationModeHighQualityCubic);

            if (FAILED(hr))
            {
                return nullptr;
            }

            // Step 5: Copy image result bytes

            int32_t numChannels = 4; // 4 bytes per pixel (e.g., 32bpp)
            int32_t bufferOutputLenght = newSize * newSize * numChannels;
            WSS::Buffer outputBuffer(bufferOutputLenght);
            outputBuffer.Length(bufferOutputLenght);

            hr = scaler->CopyPixels(
                NULL,
                newSize * numChannels,
                bufferOutputLenght,
                outputBuffer.data());

            if (FAILED(hr))
            {
                return nullptr;
            }

            return outputBuffer;
        }

        std::optional<WSS::IBuffer> DecodeAndScaleImageFromFileThumbnail(std::wstring_view path, int32_t newSize)
        {
            if (!TryInitialize())
            {
                return std::nullopt;
            }

            // Step 1: Get Thumbnail from file

            winrt::com_ptr<IShellItem> item;
            winrt::com_ptr<ISharedBitmap> pSharedBitmap;
            std::wstring pathCpy(path);
            HRESULT hr = SHCreateItemFromParsingName(pathCpy.c_str(), nullptr, IID_PPV_ARGS(&item));
            if (SUCCEEDED(hr))
            {
                winrt::com_ptr<IThumbnailCache> cache;
                hr = CoCreateInstance(__uuidof(LocalThumbnailCache), nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&cache));
                if (SUCCEEDED(hr))
                {
                    WTS_CACHEFLAGS flagsOut;
                    WTS_THUMBNAILID id;
                    hr = cache->GetThumbnail(
                        item.get(),
                        256,                 // request 256px thumbnail
                        WTS_EXTRACT,         // Attempt to extract/generate if needed with 
                        pSharedBitmap.put(),
                        &flagsOut,
                        &id
                    );
                }
            }

            if (FAILED(hr)) return std::nullopt;

            HBITMAP hBitmap;
            hr = pSharedBitmap->GetSharedBitmap(&hBitmap);

            if (FAILED(hr)) return std::nullopt;

            // Step 2: Create WIC bitmap from thumbnail ISharedBitmap

            winrt::com_ptr<IWICBitmap> pOriginalBitmap;
            hr = m_iWICImagingFactory->CreateBitmapFromHBITMAP(
                hBitmap,
                nullptr,
                WICBitmapUseAlpha,
                pOriginalBitmap.put()
            );


            if (FAILED(hr)) return std::nullopt;

            // Step 3: Format convert the frame to 32bppRGBA

            winrt::com_ptr<IWICFormatConverter> pConverter = nullptr;
            hr = m_iWICImagingFactory->CreateFormatConverter(pConverter.put());

            if (FAILED(hr))
            {
                return std::nullopt;
            }

            WICPixelFormatGUID pixelFormatGUID = GUID_WICPixelFormat32bppBGRA;

            hr = pConverter->Initialize(
                pOriginalBitmap.get(),          // Input bitmap to convert
                pixelFormatGUID,                // Destination pixel format
                WICBitmapDitherTypeNone,        // Specified dither pattern
                NULL,                           // Specify a particular palette 
                0.0f,                           // Alpha threshold
                WICBitmapPaletteTypeCustom);    // Palette translation type

            if (FAILED(hr))
            {
                return std::nullopt;
            }

            // Step 4: Scale image to target size

            winrt::com_ptr<IWICBitmapScaler> pScaler;
            hr = m_iWICImagingFactory->CreateBitmapScaler(pScaler.put());

            if (FAILED(hr)) return std::nullopt;

            hr = pScaler->Initialize(
                pConverter.get(),
                newSize,
                newSize,
                WICBitmapInterpolationMode::WICBitmapInterpolationModeHighQualityCubic);

            if (FAILED(hr))
            {
                return nullptr;
            }

            // Step 5: Copy image result bytes

            int32_t numChannels = 4; // 4 bytes per pixel (e.g., 32bpp)
            int32_t bufferOutputLenght = newSize * newSize * numChannels;
            WSS::Buffer outputBuffer(bufferOutputLenght);
            outputBuffer.Length(bufferOutputLenght);

            hr = pScaler->CopyPixels(
                NULL,
                newSize * numChannels,
                bufferOutputLenght,
                outputBuffer.data());

            if (FAILED(hr))
            {
                return nullptr;
            }

            return outputBuffer;
        }

        private:
            winrt::com_ptr<IWICImagingFactory2> m_iWICImagingFactory = nullptr;
            bool m_isInitialized = false;
    };
}
