#include "Shaders.h"

std::unique_ptr<D3D11_INPUT_ELEMENT_DESC[]> InputLayout::CreateInputLayout(const InputLayout* semantics, UINT elements)
{
	std::unique_ptr<D3D11_INPUT_ELEMENT_DESC[]> ied = std::make_unique<D3D11_INPUT_ELEMENT_DESC[]>(elements);
	for (UINT i = 0; i < elements; ++i)
	{
		ied[i] = { semantics[i].semantic, semantics[i].index, semantics[i].format, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	}
	return std::move(ied);
}

void VertexShader::Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	this->device = device;
	this->deviceContext = deviceContext;
}

HRESULT VertexShader::CreateShader(const std::wstring& path, const std::initializer_list<InputLayout>& semantics)
{
	std::wstring shaderFolder = L"";

	if (IsDebuggerPresent() == TRUE)
	{
#ifdef _DEBUG
#ifdef _WIN64
		shaderFolder = L"..\\x64\\Debug\\";
#else
		shaderFolder = L"..\\Debug\\";
#endif
#else
#ifdef _WIN64
		shaderFolder = L"..\\x64\\Release\\";
#else
		shaderFolder = L"..\\Release\\";
#endif
#endif
	}
	else
	{
		ErrorLogger::Log("Not visual studio");
	}

	if (this->buffer.Get() != nullptr)
		this->buffer.Reset();
	if (this->shader.Get() != nullptr)
		this->shader.Reset();
	if (this->inputLayout.Get() != nullptr)
		this->inputLayout.Reset();

	HRESULT hr = D3DReadFileToBlob((shaderFolder + path).c_str(), this->buffer.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, L"Faild to load shader: " + path);
		return hr;
	}
	hr = device->CreateVertexShader(this->buffer->GetBufferPointer(), this->buffer->GetBufferSize(), NULL, this->shader.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, L"Faild to create vertex shader: " + path);
		return hr;
	}

	std::unique_ptr<D3D11_INPUT_ELEMENT_DESC[]> out = InputLayout::CreateInputLayout(semantics.begin(), semantics.size());
	hr = device->CreateInputLayout(out.get(), semantics.size(), this->buffer->GetBufferPointer(), this->buffer->GetBufferSize(), this->inputLayout.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, "Failed to create input layout.");
		return hr;
	}

	return hr;
}

ID3D11VertexShader* VertexShader::GetShader()
{
	return this->shader.Get();
}

ID3D10Blob* VertexShader::GetBuffer()
{
	return this->buffer.Get();
}

ID3D11InputLayout* VertexShader::GetInputLayout()
{
	return this->inputLayout.Get();
}

void VertexShader::Use()
{
	this->deviceContext->IASetInputLayout(this->inputLayout.Get());
	this->deviceContext->VSSetShader(this->shader.Get(), NULL, 0);
}


void PixelShader::Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	this->device = device;
	this->deviceContext = deviceContext;
}

HRESULT PixelShader::CreateShader(const std::wstring& path)
{
	std::wstring shaderFolder = L"";

	if (IsDebuggerPresent() == TRUE)
	{
#ifdef _DEBUG
#ifdef _WIN64
		shaderFolder = L"..\\x64\\Debug\\";
#else
		shaderFolder = L"..\\Debug\\";
#endif
#else
#ifdef _WIN64
		shaderFolder = L"..\\x64\\Release\\";
#else
		shaderFolder = L"..\\Release\\";
#endif
#endif
	}
	

	if (this->buffer.Get() != nullptr)
		this->buffer.Reset();
	if (this->shader.Get() != nullptr)
		this->shader.Reset();

	HRESULT hr = D3DReadFileToBlob((shaderFolder + path).c_str(), this->buffer.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, L"Faild to load shader: " + path);
		return hr;
	}
	hr = device->CreatePixelShader(this->buffer->GetBufferPointer(), this->buffer->GetBufferSize(), NULL, this->shader.GetAddressOf());
	if (FAILED(hr))
	{
		ErrorLogger::Log(hr, L"Faild to create pixel shader: " + path);
		return hr;
	}
	return hr;
}

ID3D11PixelShader* PixelShader::GetShader()
{
	return this->shader.Get();
}

ID3D10Blob* PixelShader::GetBuffer()
{
	return this->buffer.Get();
}

void PixelShader::Use()
{
	this->deviceContext->PSSetShader(this->shader.Get(), NULL, 0);
}
