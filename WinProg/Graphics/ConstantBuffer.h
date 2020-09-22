#pragma once
#include <d3d11.h>
#include <wrl/client.h>
#include <memory>

template < typename T >
class ConstantBuffer
{
private:
	ConstantBuffer(const ConstantBuffer<T>& rhs);

public:
	static_assert(sizeof(T) % 16 == 0, "Data struct in constant buffer must have alignas 16");

	using ValueType = T;
	using ReferenceType = T&;
	using PointerType = T*;

	T data;

	ConstantBuffer() {}

	HRESULT CreateBuffer(const T& data)
	{
		if (buffer.Get() != nullptr)
			buffer.Reset();

		this->data = data;

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		bufferDesc.ByteWidth = sizeof(T);
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA constantBufferData;
		ZeroMemory(&constantBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
		constantBufferData.pSysMem = &this->data;

		HRESULT hr = this->device->CreateBuffer(&bufferDesc, &constantBufferData, this->buffer.GetAddressOf());
		if (FAILED(hr))
		{
			ErrorLogger::Log(hr, "Failed to create constant buffer.");
			return hr;
		}
		return hr;
	}

	HRESULT ApplyChanges()
	{
		D3D11_MAPPED_SUBRESOURCE mappedSubresource;
		HRESULT hr = this->deviceContext->Map(this->buffer.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
		if (FAILED(hr))
		{
			ErrorLogger::Log(hr, "Failed to apply changes in constant buffer.");
			return hr;
		}
		CopyMemory(mappedSubresource.pData, &this->data, sizeof(T));
		this->deviceContext->Unmap(this->buffer.Get(), 0);
		return hr;
	}

	ID3D11Buffer* Get() const { return this->buffer.Get(); }
	ID3D11Buffer*const* GetAddressOf() const { return this->buffer.GetAddressOf(); }
	UINT BufferSize() const { return this->bufferSize; }

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;

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