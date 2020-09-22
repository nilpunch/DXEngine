#pragma once
#include "GraphicsInterface.h"

#include <d3d11.h>
#include <wrl/client.h>
#include <memory>

class IndexBuffer : public IUsable
{
	IndexBuffer(const IndexBuffer& rhs);

public:
	IndexBuffer() {}

	HRESULT CreateBuffer(const DWORD* indecis, UINT size)
	{
		if (buffer.Get() != nullptr)
			buffer.Reset();

		this->bufferSize = size;

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(DWORD) * this->bufferSize;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA indexBufferData;
		ZeroMemory(&indexBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
		indexBufferData.pSysMem = indecis;

		return this->device->CreateBuffer(&bufferDesc, &indexBufferData, this->buffer.GetAddressOf());
	}
	HRESULT CreateBuffer(std::initializer_list<DWORD> indices)
	{
		return this->CreateBuffer(indices.begin(), indices.size());
	}

	ID3D11Buffer* Get() const { return this->buffer.Get(); }
	ID3D11Buffer*const* GetAddressOf() const { return this->buffer.GetAddressOf(); }
	UINT Count() const { return this->bufferSize; }

	virtual void Use() override
	{
		this->deviceContext->IASetIndexBuffer(this->buffer.Get(), Format, 0);
	}

	const DXGI_FORMAT Format = DXGI_FORMAT_R32_UINT;


private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	UINT bufferSize = 0;

public:
	void Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
	{
		this->device = device;
		this->deviceContext = deviceContext;
	}

private:
	ID3D11Device* device = nullptr;
	ID3D11DeviceContext* deviceContext = nullptr;

};