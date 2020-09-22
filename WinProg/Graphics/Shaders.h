#pragma once
#include "GraphicsInterface.h"
#include "..\\ErrorLogger.h"
#pragma comment (lib, "D3DCompiler.lib")
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <initializer_list>

struct InputLayout
{
	static std::unique_ptr<D3D11_INPUT_ELEMENT_DESC[]> CreateInputLayout(const InputLayout* semantics, UINT elements);
	LPCSTR semantic;
	DXGI_FORMAT format;
	UINT index;
};

class VertexShader : public IUsable
{
public:
	void Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	HRESULT CreateShader(const std::wstring& path, const std::initializer_list<InputLayout>& semantics);
	ID3D11VertexShader* GetShader();
	ID3D10Blob* GetBuffer();
	ID3D11InputLayout* GetInputLayout();

	virtual void Use() override;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	shader;
	Microsoft::WRL::ComPtr<ID3D10Blob>			buffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	inputLayout;

	ID3D11DeviceContext* deviceContext = nullptr;
	ID3D11Device* device = nullptr;
};

class PixelShader : public IUsable
{
public:
	void Initialise(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	HRESULT CreateShader(const std::wstring& path);
	ID3D11PixelShader* GetShader();
	ID3D10Blob* GetBuffer();

	virtual void Use() override;

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	shader;
	Microsoft::WRL::ComPtr<ID3D10Blob>			buffer;

	ID3D11DeviceContext* deviceContext = nullptr;
	ID3D11Device* device = nullptr;
};