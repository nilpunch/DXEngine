#pragma once
#include "GraphicsInterface.h"

#include <d3d11.h>
#include <wrl/client.h>
#include <memory>
#include <initializer_list>

template < typename VertexType >
class VertexBuffer : public IUsable
{
private:
	VertexBuffer(const VertexBuffer<VertexType>& rhs);

public:
	VertexBuffer() {}

	HRESULT CreateBuffer(const VertexType* data, UINT vertices)
	{
		if (buffer.Get() != nullptr)
			buffer.Reset();

		this->bufferSize = vertices;
		this->stride = std::make_unique<UINT>(sizeof(VertexType));

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(VertexType) * this->bufferSize;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexBufferData;
		ZeroMemory(&vertexBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
		vertexBufferData.pSysMem = data;

		HRESULT hr = this->device->CreateBuffer(&bufferDesc, &vertexBufferData, this->buffer.GetAddressOf());
		if (FAILED(hr))
		{
			ErrorLogger::Log(hr, "Failed to create vertex buffer.");
			return hr;
		}
		return hr;
	}

	HRESULT CreateBuffer(std::initializer_list<VertexType> vertices)
	{
		return this->CreateBuffer(vertices.begin(), vertices.size());
	}

	ID3D11Buffer* Get() const					{ return this->buffer.Get(); }
	ID3D11Buffer*const* GetAddressOf() const	{ return this->buffer.GetAddressOf(); }
	UINT VerticesCount() const					{ return this->bufferSize; }
	UINT Stride() const							{ return *(this->stride.get()); }
	const UINT* StridePtr() const				{ return this->stride.get(); }

	virtual void Use() override
	{
		this->deviceContext->IASetVertexBuffers(0, 1, this->buffer.GetAddressOf(), this->stride.get(), &this->offset);
	}

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	std::unique_ptr<UINT> stride;
	UINT bufferSize;

	const UINT offset = 0;

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

template < typename VertexType >
class VertexBufferDynamic : public IUsable
{
private:
	VertexBufferDynamic(const VertexBuffer<VertexType>& rhs);

public:
	VertexBufferDynamic() {}

	HRESULT CreateBuffer(const VertexType* data, UINT vertices)
	{
		if (buffer.Get() != nullptr)
			buffer.Reset();

		this->bufferSize = vertices;
		this->stride = std::make_unique<UINT>(sizeof(VertexType));

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = sizeof(VertexType) * this->bufferSize;
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA vertexBufferData;
		ZeroMemory(&vertexBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
		vertexBufferData.pSysMem = data;

		HRESULT hr = this->device->CreateBuffer(&bufferDesc, &vertexBufferData, this->buffer.GetAddressOf());
		if (FAILED(hr))
		{
			ErrorLogger::Log(hr, "Failed to create dynamic vertex buffer.");
			return hr;
		}
		return hr;
	}

	HRESULT CreateBuffer(std::initializer_list<VertexType> vertices)
	{
		return this->CreateBuffer(vertices.begin(), vertices.size());
	}

	ID3D11Buffer* Get() const { return this->buffer.Get(); }
	ID3D11Buffer*const* GetAddressOf() const { return this->buffer.GetAddressOf(); }
	UINT VerticesCount() const { return this->bufferSize; }
	UINT Stride() const { return *(this->stride.get()); }
	const UINT* StridePtr() const { return this->stride.get(); }

	HRESULT SetVertices(const VertexType* data)
	{
		D3D11_MAPPED_SUBRESOURCE mappedSubresource;
		HRESULT hr = this->deviceContext->Map(this->buffer.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
		if (FAILED(hr))
		{
			ErrorLogger::Log(hr, "Failed to apply changes in constant buffer.");
			return hr;
		}
		CopyMemory(mappedSubresource.pData, data, sizeof(VertexType) * this->bufferSize);
		this->deviceContext->Unmap(this->buffer.Get(), 0);
		return hr;
	}

	virtual void Use() override
	{
		this->deviceContext->IASetVertexBuffers(0, 1, this->buffer.GetAddressOf(), this->stride.get(), &this->offset);
	}

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	std::unique_ptr<UINT> stride;
	UINT bufferSize = 0;
	const UINT offset = 0;

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