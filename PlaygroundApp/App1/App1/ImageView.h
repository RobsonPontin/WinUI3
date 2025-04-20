#pragma once

#include <wincodec.h>
#include <exception>

#include <winrt/Windows.Storage.Streams.h>

namespace Playground
{
    struct ImageView
    {
        ImageView() = default;

        ImageView(
            std::int32_t width,
            std::int32_t height,
            std::vector<uint8_t> pixelArray,
            WICPixelFormatGUID pixelFormat)
        {
            if (width < 0 || height < 0)
            {
                throw std::exception("Invalid image dimension");
            }

            m_width = width;
            m_height = height;
            m_pixelFormat = pixelFormat;

            auto sizeInPixels = static_cast<size_t>(width) * static_cast<size_t>(height);
            auto sizeInBytes = sizeInPixels * sizeof(uint8_t);

            if (pixelArray.size() < sizeInBytes)
            {
                throw std::exception("Buffer size too small for image passed");
            }

            m_pixels = pixelArray;
        }

        uint32_t Width()
        {
            return m_width;
        }

        uint32_t Height()
        {
            return m_height;
        }

        std::vector<std::uint8_t> Pixels()
        {
            return m_pixels;
        }

        WICPixelFormatGUID PixelFormatWIC()
        {
            return m_pixelFormat;
        }

    private:
        WF::IMemoryBufferReference m_memoryBuffer;
        std::vector<std::uint8_t> m_pixels;
        uint32_t m_width;
        uint32_t m_height;
        WICPixelFormatGUID m_pixelFormat{ GUID_WICPixelFormatDontCare };
    };
}