#include "stdafx.h"
#include "DirectX.h"
#include <DL_Debug.h>


DirectX::DirectX(HWND& aHwnd, SetupInfo& aSetupInfo)
	: myHWND(aHwnd)
	, mySetupInfo(aSetupInfo)
{
	D3DSetup();
}


DirectX::~DirectX()
{
}

void DirectX::Present(const unsigned int aSyncInterval, const unsigned int aFlags)
{
	mySwapChain->Present(aSyncInterval, aFlags);
	
}
void DirectX::Clear(const float aClearColor[4])
{
	myContext->OMSetRenderTargets(1, &myRenderTargetView, myDepthBufferView);
	myContext->ClearRenderTargetView(myRenderTargetView, aClearColor);
	myContext->ClearDepthStencilView(myDepthBufferView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void DirectX::OnResize(const int aWidth, const int aHeight)
{
	myContext->OMSetRenderTargets(0, NULL, NULL);
	myRenderTargetView->Release();
	myContext->Flush();
	mySwapChain->ResizeBuffers(1, aWidth, aHeight, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

	D3DSwapChainSetup();
	D3DRenderTargetSetup();
	D3DViewPortSetup(aWidth, aHeight);
	D3DStencilBufferSetup(aWidth, aHeight);
}

void DirectX::CleanD3D()
{
	mySwapChain->SetFullscreenState(FALSE, NULL);

	mySwapChain->Release();
	mySwapChain = nullptr;

	myRenderTargetView->Release();
	myRenderTargetView = nullptr;

	myDevice->Release();
	myDevice = nullptr;

	myContext->Release();
	myContext = nullptr;
}

bool DirectX::D3DSetup()
{
	if (D3DSwapChainSetup() == false)
	{
		DIRECTX_LOG("Failed to Setup DirectX SwapChain");
		return false;
	}

	if (D3DRenderTargetSetup() == false)
	{
		DIRECTX_LOG("Failed to Setup RenderTarget");
		return false;
	}

	if (D3DViewPortSetup(mySetupInfo.myScreenWidth, mySetupInfo.myScreenHeight) == false)
	{
		DIRECTX_LOG("Failed to Setup DirectX ViewPort");
		return false;
	}

	if (D3DStencilBufferSetup(mySetupInfo.myScreenWidth, mySetupInfo.myScreenHeight) == false)
	{
		DIRECTX_LOG("Failed to Setup DirectX Stencil Buffer");
		return false;
	}

	
	DIRECTX_LOG("DirectX Setup Successful");
	return true;
}

bool DirectX::D3DRenderTargetSetup()
{
	ID3D11Texture2D* backBuffer;
	mySwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);

	myDevice->CreateRenderTargetView(backBuffer, NULL, &myRenderTargetView);
	backBuffer->Release();

	myContext->OMSetRenderTargets(1, &myRenderTargetView, NULL);

	return TRUE;
}

bool DirectX::D3DSwapChainSetup()
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = mySetupInfo.myScreenWidth;
	swapChainDesc.BufferDesc.Height = mySetupInfo.myScreenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = myHWND;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.Windowed = mySetupInfo.myWindowed;

	HRESULT result = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&mySwapChain,
		&myDevice,
		NULL,
		&myContext);

	if (FAILED(result))
	{
		return false;
	}

	return TRUE;
}

bool DirectX::D3DViewPortSetup(int aWidth, int aHeight)
{
	D3D11_VIEWPORT viewPort;
	ZeroMemory(&viewPort, sizeof(D3D11_VIEWPORT));

	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Width = static_cast<FLOAT>(aWidth);
	viewPort.Height = static_cast<FLOAT>(aHeight);
	viewPort.MinDepth = 0.f;
	viewPort.MaxDepth = 1.f;

	myContext->RSSetViewports(1, &viewPort);

	return true;
}

bool DirectX::D3DStencilBufferSetup(int aWidth, int aHeight)
{
	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC depthBufferInfo;
	ZeroMemory(&depthBufferInfo, sizeof(depthBufferInfo));

	depthBufferInfo.Width = aWidth;
	depthBufferInfo.Height = aHeight;
	depthBufferInfo.MipLevels = 1;
	depthBufferInfo.ArraySize = 1;
	depthBufferInfo.Format = DXGI_FORMAT_D32_FLOAT;
	depthBufferInfo.SampleDesc.Count = 1;
	depthBufferInfo.Usage = D3D11_USAGE_DEFAULT;
	depthBufferInfo.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	hr = myDevice->CreateTexture2D(&depthBufferInfo, NULL, &myDepthBuffer);
	if (FAILED(hr))
	{
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC stencilDesc;
	ZeroMemory(&stencilDesc, sizeof(stencilDesc));

	stencilDesc.Format = depthBufferInfo.Format;
	stencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

	stencilDesc.Texture2D.MipSlice = 0;

	hr = myDevice->CreateDepthStencilView(myDepthBuffer, &stencilDesc, &myDepthBufferView);
	if (FAILED(hr))
	{
		return false;
	}

	return true;
}