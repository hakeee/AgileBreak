#include "wdgraphics.h"
#include <dxgidebug.h>
#include "directx/dxshader.h"
#include "directx/dxbuffer.h"
#include "directx/dxtexture.h"
#include "wwindow.h"
#include "shader.h"
using namespace discobreak;

Wdgraphics::Wdgraphics(Wwindow* window) {

	windowWidth_ = window->windowWidth_;
	windowHeight_ = window->windowHeight_;

	InitDeviceAndSwapChain(window->hWnd_);
	CreateRenderTargetView();
	CreateDepthStencilView();
	CreateRasterizerState();
	SetViewport();


}

Wdgraphics::~Wdgraphics() {

	if (deviceContext_)
		deviceContext_->ClearState();
	if (renderTarget_)
		renderTarget_->Release();
	if (depthStencilView_)
		depthStencilView_->Release();
	if (depthStencilBuffer_)
		depthStencilBuffer_->Release();
	if (rasterizerState_)
		rasterizerState_->Release();
	if (swapChain_)
		swapChain_->Release();
	if (deviceContext_)
		deviceContext_->Release();
	if (device_)
		device_->Release();
}

HRESULT Wdgraphics::InitDeviceAndSwapChain(HWND hWnd) {
	HRESULT hr = E_FAIL;
	UINT flags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif 
	D3D_DRIVER_TYPE driverTypes[] = { D3D_DRIVER_TYPE_HARDWARE, D3D_DRIVER_TYPE_REFERENCE };
	D3D_FEATURE_LEVEL featureLevels[] = { D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL usedFeatureLevel = D3D_FEATURE_LEVEL_11_0;

	DXGI_SWAP_CHAIN_DESC scDesc;
	scDesc.BufferDesc.Width = windowWidth_;
	scDesc.BufferDesc.Height = windowHeight_;
	scDesc.BufferDesc.RefreshRate.Numerator = 60;
	scDesc.BufferDesc.RefreshRate.Denominator = 1;
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	scDesc.SampleDesc.Count = 1;
	scDesc.SampleDesc.Quality = 0;

	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;// | DXGI_USAGE_UNORDERED_ACCESS;
	scDesc.BufferCount = 1;
	scDesc.OutputWindow = hWnd;
	scDesc.Windowed = true;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scDesc.Flags = 0;

	for (UINT driverType = 0; driverType < ARRAYSIZE(driverTypes) && FAILED(hr); driverType++) {
		hr = D3D11CreateDeviceAndSwapChain(
			nullptr,
			driverTypes[driverType],
			nullptr,
			flags,
			featureLevels,
			ARRAYSIZE(featureLevels),
			D3D11_SDK_VERSION,
			&scDesc,
			&swapChain_,
			&device_,
			&usedFeatureLevel,
			&deviceContext_);

		if (FAILED(hr)) {
			MessageBox(0, "D3D11CreateDeviceAndSwapChain Failed!", 0, 0);
			return hr;
		}
	}


	if (usedFeatureLevel != D3D_FEATURE_LEVEL_11_0) {
		MessageBox(0, "Direct3D Feature level 11 is not supported", 0, 0);
		return hr;
	}
	

	return hr;
}

HRESULT Wdgraphics::CreateRenderTargetView() {
	HRESULT hr = S_OK;

	ID3D11Texture2D* backBufferPointer;
	if (SUCCEEDED(hr = swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPointer))) {
		hr = device_->CreateRenderTargetView(backBufferPointer, nullptr, &renderTarget_);
		if (FAILED(hr)) {
			MessageBox(0, "Failed to create render target view", 0, 0);
			return hr;
		}
		backBufferPointer->Release();
	}
	return hr;
}

HRESULT Wdgraphics::CreateDepthStencilView() {
	HRESULT hr;

	D3D11_TEXTURE2D_DESC desc;
	desc.Width = windowWidth_;
	desc.Height = windowHeight_;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_D32_FLOAT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;

	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	if (FAILED(hr = device_->CreateTexture2D(&desc, nullptr, &depthStencilBuffer_))) {
		MessageBox(0, "Failed to create depth stencil buffer", 0, 0);
		return hr;
	}
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = desc.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	if (FAILED(hr = device_->CreateDepthStencilView(depthStencilBuffer_, &descDSV, &depthStencilView_))) {
		MessageBox(0, "Failed to create depth stencil view", 0, 0);
		return hr;
	}

	deviceContext_->OMSetRenderTargets(1, &renderTarget_, depthStencilView_);

	return S_OK;
}

HRESULT Wdgraphics::CreateRasterizerState() {
	D3D11_RASTERIZER_DESC rDesc;
	rDesc.FillMode = D3D11_FILL_SOLID;
	rDesc.CullMode = D3D11_CULL_NONE;
	rDesc.FrontCounterClockwise = TRUE;
	rDesc.DepthBias = 0;
	rDesc.SlopeScaledDepthBias = 0.0f;
	rDesc.DepthBiasClamp = 0.0f;
	rDesc.DepthClipEnable = TRUE;
	rDesc.ScissorEnable = FALSE;
	rDesc.MultisampleEnable = TRUE;
	rDesc.AntialiasedLineEnable = TRUE;

	return device_->CreateRasterizerState(&rDesc, &rasterizerState_);
}

void Wdgraphics::SetViewport() {
	viewPort_.Width = (float)windowWidth_;
	viewPort_.Height = (float)windowHeight_;
	viewPort_.MinDepth = 0.0f;
	viewPort_.MaxDepth = 1.0f;
	viewPort_.TopLeftX = 0;
	viewPort_.TopLeftY = 0;
	deviceContext_->RSSetViewports(1, &viewPort_);
}


void Wdgraphics::Swap() {

	swapChain_->Present(0, 0);
}
void discobreak::Wdgraphics::Clear(float r, float g, float b, float a) {
    float clearColor[4] = { r, g, b, a };

	deviceContext_->OMSetRenderTargets(1, &renderTarget_, depthStencilView_);


	deviceContext_->ClearRenderTargetView(renderTarget_, clearColor);
	deviceContext_->ClearDepthStencilView(depthStencilView_, D3D11_CLEAR_DEPTH, 1, 0);
	deviceContext_->OMSetDepthStencilState(NULL, 0);
	deviceContext_->RSSetState(rasterizerState_);
};

Buffer* discobreak::Wdgraphics::NewBuffer() {
	return new Buffer(new DxBuffer(this));
}

Shader* discobreak::Wdgraphics::NewShader() {
	return (new Shader(new DxShader(this)));
}

Texture* discobreak::Wdgraphics::NewTexture() {
	return (new Texture(new DxTexture(this)));
}

void discobreak::Wdgraphics::PrintVersion() {
	switch (device_->GetFeatureLevel()) {
	case 0x9100:
		printf("D3D_FEATURE_LEVEL_9_1");
		break;
	case 0x9200:
		printf("D3D_FEATURE_LEVEL_9_2");
		break;
	case 0x9300:
		printf("D3D_FEATURE_LEVEL_9_3");
		break;
	case 0xa000:
		printf("D3D_FEATURE_LEVEL_10_0");
		break;
	case 0xa100:
		printf("D3D_FEATURE_LEVEL_10_1");
		break;
	case 0xb000:
		printf("D3D_FEATURE_LEVEL_11_0");
		break;
	case 0xb100:
		printf("D3D_FEATURE_LEVEL_11_1");
		break;
	case 0xc000:
		printf("D3D_FEATURE_LEVEL_12_0");
		break;
	case 0xc100:
		printf("D3D_FEATURE_LEVEL_12_1");
		break;
	default:
		break;
	}
}
