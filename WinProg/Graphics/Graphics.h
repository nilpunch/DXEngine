#pragma once
#include "AdapterReader.h"
#include "Shaders.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Camera.h"
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>

class Graphics
{
public:
	bool Initialise(HWND hWnd, int width, int height);
	void RenderPreparation();
	void SwapChain();

	ID3D11Device* Device() { return this->device.Get(); }
	ID3D11DeviceContext* DeviceContext() { return this->deviceContext.Get(); }
	ID3D11RenderTargetView* RenderTargetView() { return this->renderTargetView.Get(); }

private:
	bool InitialiseDirectX(HWND hWnd);

	Microsoft::WRL::ComPtr<ID3D11Device>			device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			swapchain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	renderTargetView;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			depthStencilBuffer;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState>	depthStencilState;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	rasterizerState;

	std::unique_ptr<DirectX::SpriteBatch>			spriteBatch;
	std::unique_ptr<DirectX::SpriteFont>			spriteFont;

	Microsoft::WRL::ComPtr<ID3D11SamplerState>		samplerState;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> myTexture;

	int windowWidth = 0;
	int windowHeight = 0;
};