#pragma once

#include <d2d1.h>
#include <d2d1_3.h>
#include <d2d1effects.h>
#include <wincodec.h>
#include <comdef.h>

namespace Playground
{
    struct D2DImageResizer
    {
        D2DImageResizer()
        {
        }

        bool TryInitialize(HWND hWnd)
        {
            if (m_isInitialized)
            {
                return m_isInitialized;
            }

            // Initialize Direct2D
            D2D1_FACTORY_OPTIONS options;
            ZeroMemory(&options, sizeof(options));

            auto hr = D2D1CreateFactory(
                D2D1_FACTORY_TYPE_SINGLE_THREADED, 
                __uuidof(ID2D1Factory6),
                &options,
                m_d2DFactory.put_void());

            // Initialize WIC
            if (SUCCEEDED(hr))
            {
                hr = CoCreateInstance(
                    CLSID_WICImagingFactory2,
                    nullptr,
                    CLSCTX_INPROC_SERVER,
                    IID_PPV_ARGS(&m_iWICImagingFactory));
            }

            // Create a render target
            if (SUCCEEDED(hr))
            {
                RECT rc;
                GetClientRect(hWnd, &rc);

                D2D1_SIZE_U size = D2D1::SizeU(rc.right - rc.left, rc.bottom - rc.top);

                hr = m_d2DFactory->CreateHwndRenderTarget(
                    D2D1::RenderTargetProperties(),
                    D2D1::HwndRenderTargetProperties(hWnd, size),
                    m_iD2D1HwndRenderTarget.put());
            }

            if (SUCCEEDED(hr))
            {
                return m_isInitialized = true;
            }

            return false;
        }

        void ScaleImage(std::wstring_view const& path)
        {
            if (!m_isInitialized)
            {
                return;
            }

            PCWSTR uri = path.data();

            ID2D1Bitmap* pBitmap = nullptr;
            IWICBitmapDecoder* pDecoder = nullptr;
            IWICBitmapFrameDecode* pFrame = nullptr;
            IWICFormatConverter* pConverter = nullptr;

            HRESULT hr = m_iWICImagingFactory->CreateDecoderFromFilename(
                uri, 
                nullptr, 
                GENERIC_READ,
                WICDecodeMetadataCacheOnLoad, 
                &pDecoder);

            if (SUCCEEDED(hr))
            {
                hr = pDecoder->GetFrame(0, &pFrame);
            }

            if (SUCCEEDED(hr))
            {
                hr = m_iWICImagingFactory->CreateFormatConverter(&pConverter);
            }

            if (SUCCEEDED(hr))
            {
                hr = pConverter->Initialize(
                    pFrame,
                    GUID_WICPixelFormat32bppPBGRA,
                    WICBitmapDitherTypeNone,
                    nullptr,
                    0.0f,
                    WICBitmapPaletteTypeMedianCut);
            }

            if (SUCCEEDED(hr))
            {
                hr = m_iD2D1HwndRenderTarget->CreateBitmapFromWicBitmap(
                    pConverter, 
                    nullptr, 
                    &pBitmap);
            }

            if (pDecoder) pDecoder->Release();
            if (pFrame) pFrame->Release();
            if (pConverter) pConverter->Release();
        }

        private:
            winrt::com_ptr<ID2D1Factory> m_d2DFactory = nullptr;
            winrt::com_ptr<ID2D1HwndRenderTarget> m_iD2D1HwndRenderTarget = nullptr;
            winrt::com_ptr<IWICImagingFactory2> m_iWICImagingFactory = nullptr; // WIC uses CPU
            bool m_isInitialized = false;
    };
}
