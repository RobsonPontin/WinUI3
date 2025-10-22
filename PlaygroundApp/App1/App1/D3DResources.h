#pragma once

#include <d3d11.h>
#include <dxgi1_2.h>
#include <windows.graphics.directx.direct3d11.interop.h>

#include <winrt/Windows.Graphics.DirectX.h>
#include <winrt/Windows.Graphics.DirectX.Direct3D11.h>
#include <winrt/Windows.Media.Core.h>
#include <winrt/Windows.Media.Editing.h>
#include <winrt/Windows.Media.Playback.h>
#include <winrt/Windows.Storage.h>

namespace Playground
{
	struct D3DResources
	{
		winrt::com_ptr<ID3D11Device>           m_d3dDevice;
		winrt::com_ptr<ID3D11DeviceContext>    m_d3dDeviceContext;
		winrt::com_ptr<ID3D11Texture2D>        m_d3dTexture2D;
		winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DSurface  m_frameSurface{ nullptr }; // WinRT wrapper

		static D3DResources Create(uint32_t width, uint32_t height)
		{
			D3DResources r{};

			UINT flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if defined(_DEBUG)
			flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

			D3D_FEATURE_LEVEL flRequested[] = {
				D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0
			};

			winrt::com_ptr<ID3D11Device> dev;
			winrt::com_ptr<ID3D11DeviceContext> ctx;
			D3D_FEATURE_LEVEL flCreated{};

			winrt::check_hresult(D3D11CreateDevice(
				nullptr,                       // adapter
				D3D_DRIVER_TYPE_HARDWARE,
				nullptr,                       // software
				flags,
				flRequested, _countof(flRequested),
				D3D11_SDK_VERSION,
				dev.put(),
				&flCreated,
				ctx.put()
			));

			r.m_d3dDevice = dev;
			r.m_d3dDeviceContext = ctx;

			// Create a BGRA surface compatible with MediaPlayer frame copy
			D3D11_TEXTURE2D_DESC desc{};
			desc.Width = width;
			desc.Height = height;
			desc.MipLevels = 1;
			desc.ArraySize = 1;
			desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; // common choice
			desc.SampleDesc.Count = 1;
			desc.Usage = D3D11_USAGE_DEFAULT;
			desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = D3D11_RESOURCE_MISC_SHARED; // often useful

			winrt::check_hresult(r.m_d3dDevice->CreateTexture2D(&desc, nullptr, r.m_d3dTexture2D.put()));

			// Wrap the DXGI surface as IDirect3DSurface
			winrt::com_ptr<IDXGIResource> dxgiResource;
			r.m_d3dTexture2D.as(dxgiResource);

			winrt::com_ptr<IDXGISurface> dxgiSurface;
			r.m_d3dTexture2D.as(dxgiSurface);

			// Convert to WinRT surface
			winrt::com_ptr<WF::IInspectable> inspectableSurface;
			winrt::check_hresult(CreateDirect3D11SurfaceFromDXGISurface(
				dxgiSurface.get(),
				reinterpret_cast<IInspectable**>(inspectableSurface.put_void())
			));
			r.m_frameSurface = inspectableSurface.as<winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DSurface>();

			return r;
		}

		WF::Rect GetFrameSize() const
		{
			auto desc = m_frameSurface.Description();
			return WF::Rect{ 0, 0, static_cast<float>(desc.Width), static_cast<float>(desc.Height) };
		}

		// Optional: expose a WinRT IDirect3DDevice if you need one elsewhere
		winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice AsWinRTDevice() const
		{
			winrt::com_ptr<IDXGIDevice> dxgiDevice;
			m_d3dDevice.as(dxgiDevice);

			winrt::com_ptr<IInspectable> inspectableDev;
			winrt::check_hresult(CreateDirect3D11DeviceFromDXGIDevice(
				dxgiDevice.get(),
				reinterpret_cast<IInspectable**>(inspectableDev.put_void())
			));
			return inspectableDev.as<winrt::Windows::Graphics::DirectX::Direct3D11::IDirect3DDevice>();
		}
	};
}