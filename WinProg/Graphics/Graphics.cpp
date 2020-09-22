#include "Graphics.h"

bool Graphics::Initialise(HWND hWnd, int width, int height)
{
	this->windowWidth = width;
	this->windowHeight = height;

	if (!InitialiseDirectX(hWnd))
		return false;

	return true;
}

//void Graphics::RenderPreparation()
//{
//	float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
//	this->deviceContext->ClearRenderTargetView(this->renderTargetView.Get(), color);
//	this->deviceContext->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
//
//	this->deviceContext->IASetInputLayout(this->vertexShader.GetInputLayout());
//	this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	this->deviceContext->RSSetState(this->rasterizerState.Get());
//	this->deviceContext->OMSetDepthStencilState(this->depthStencilState.Get(), 0);
//	this->deviceContext->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());
//
//	this->deviceContext->VSSetShader(this->vertexShader.GetShader(), NULL, 0);
//	this->deviceContext->PSSetShader(this->pixelShader.GetShader(), NULL, 0);
//
//	UINT offset = 0;
//
//	this->deviceContext->PSSetShaderResources(0, 1, this->myTexture.GetAddressOf());
//	
//	vertexBuffer.Use();
//	indexBuffer.Use();
//
//	this->deviceContext->DrawIndexed(this->indexBuffer.Count(), 0, 0);
//
//	this->spriteBatch->Begin();
//	this->spriteFont->DrawString(this->spriteBatch.get(), "I am text", DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::Colors::Black, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));
//	this->spriteBatch->End();
//}

void Graphics::RenderPreparation()
{
	float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	this->deviceContext->ClearRenderTargetView(this->renderTargetView.Get(), color);
	this->deviceContext->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	this->deviceContext->RSSetState(this->rasterizerState.Get());
	this->deviceContext->OMSetDepthStencilState(this->depthStencilState.Get(), 0);
}

void Graphics::SwapChain()
{
	this->swapchain->Present(1, NULL);
}

bool Graphics::InitialiseDirectX(HWND hWnd)
{
	const std::vector<AdapterData>& adapterData = AdapterReader::GetAdapters();

	if (adapterData.size() == 0)
	{
		ErrorLogger::Log("No DXGI adapters found.");
		return false;
	}

	const AdapterData* highestVideoMemoryAdapter = nullptr;
	for (const AdapterData& adapter : adapterData)
	{
		if (highestVideoMemoryAdapter == nullptr)
		{
			highestVideoMemoryAdapter = &adapter;
		}
		else
		{
			if (adapter.description.DedicatedVideoMemory > highestVideoMemoryAdapter->description.DedicatedVideoMemory)
				highestVideoMemoryAdapter = &adapter;
		}
	}

	DXGI_SWAP_CHAIN_DESC sdc;
	ZeroMemory(&sdc, sizeof(DXGI_SWAP_CHAIN_DESC));
	sdc.BufferDesc.Width = this->windowWidth;
	sdc.BufferDesc.Height = this->windowHeight;
	sdc.BufferDesc.RefreshRate.Numerator = 60;	// TODO
	sdc.BufferDesc.RefreshRate.Denominator = 1;	// TODO
	sdc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sdc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sdc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	sdc.SampleDesc.Count = 1;	// TODO
	sdc.SampleDesc.Quality = 0;	// TODO

	sdc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sdc.BufferCount = 1;
	sdc.OutputWindow = hWnd;
	sdc.Windowed = TRUE;
	sdc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sdc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	HRESULT hr = D3D11CreateDeviceAndSwapChain(highestVideoMemoryAdapter->pAdapter, // DXGI adapter
											   D3D_DRIVER_TYPE_UNKNOWN,
											   NULL, // For software driver type
											   NULL, // Flags for runtime layers
											   NULL, // Feature levels array
											   0, // Feature levels in array
											   D3D11_SDK_VERSION,
											   &sdc, // Swapchain description
											   this->swapchain.GetAddressOf(),
											   this->device.GetAddressOf(),
											   NULL, // Supported feature levels
											   this->deviceContext.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create device adn swapchain.");
		return false;
	}

	Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
	hr = this->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()));
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to get buffer.");
		return false;
	}
	hr = this->device->CreateRenderTargetView(backBuffer.Get(), NULL, this->renderTargetView.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create render target view.");
		return false;
	}

	D3D11_TEXTURE2D_DESC depthBufferDesc;
	depthBufferDesc.Width = this->windowWidth;
	depthBufferDesc.Height = this->windowHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;
	hr = this->device->CreateTexture2D(&depthBufferDesc, NULL, this->depthStencilBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create depth stencil buffer.");
		return false;
	}
	hr = this->device->CreateDepthStencilView(this->depthStencilBuffer.Get(), NULL, depthStencilView.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create depth stencil view.");
		return false;
	}

	this->deviceContext->OMSetRenderTargets(1, this->renderTargetView.GetAddressOf(), this->depthStencilView.Get());

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;
	hr = this->device->CreateDepthStencilState(&depthStencilDesc, this->depthStencilState.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create depth stencil state.");
		return false;
	}

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = (float)this->windowWidth;
	viewport.Height = (float)this->windowHeight;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	this->deviceContext->RSSetViewports(1, &viewport);

	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
	rasterizerDesc.FrontCounterClockwise = false;
	hr = this->device->CreateRasterizerState(&rasterizerDesc, this->rasterizerState.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create rasterizer state.");
		return false;
	}

	spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->deviceContext.Get());
	spriteFont = std::make_unique<DirectX::SpriteFont>(this->device.Get(), L"Resources/Fonts/consolas_20.spritefont");

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	hr = this->device->CreateSamplerState(&samplerDesc, this->samplerState.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create sampler state.");
		return false;
	}

	return true;
}
