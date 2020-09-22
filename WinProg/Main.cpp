//#include "DirectXEngine.h"
//#define DXE_DEBUG
//#define DXE_NO_FPS_LIMIT
#include "DXEngine.h"

class DemoWindow : public DXEngine
{
private:
	struct Vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

	VertexShader vs_colorfull;
	PixelShader ps_colorfull;
	VertexShader vs_wireframe;
	PixelShader ps_wireframe;
	VertexBufferDynamic<Vertex> vbuffer;
	IndexBuffer ibuffer;
	ConstantBuffer<DirectX::XMMATRIX> cbuffer;

	Vertex qube_vertices[8]
	{
		{ DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)	},
		{ DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)	},
		{ DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f),		DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)	},
		{ DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)	},

		{ DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f)	},
		{ DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f),		DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)	},
		{ DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),		DirectX::XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f)	},
		{ DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f),		DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)	},
	};

protected:
	virtual void Init() override
	{
		vs_colorfull.Initialise(gfx.Device(), gfx.DeviceContext());
		ps_colorfull.Initialise(gfx.Device(), gfx.DeviceContext());
		vs_wireframe.Initialise(gfx.Device(), gfx.DeviceContext());
		ps_wireframe.Initialise(gfx.Device(), gfx.DeviceContext());
		vbuffer.Initialise(this->gfx.Device(), this->gfx.DeviceContext());
		ibuffer.Initialise(this->gfx.Device(), this->gfx.DeviceContext());
		cbuffer.Initialise(this->gfx.Device(), this->gfx.DeviceContext());

		HRESULT vscreation = vs_colorfull.CreateShader(L"vertex_shader.cso",
			{
				{ "POSITION", DXGI_FORMAT_R32G32B32_FLOAT, 0 },
				{ "COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT, 0 }
			});
		if (FAILED(vscreation))
			Quit();

		vscreation = vs_wireframe.CreateShader(L"wireframe_vertex.cso",
			{
				{ "POSITION", DXGI_FORMAT_R32G32B32_FLOAT, 0 },
				{ "COLOR", DXGI_FORMAT_R32G32B32A32_FLOAT, 0 }
			});
		if (FAILED(vscreation))
			Quit();

		if (FAILED(ps_colorfull.CreateShader(L"pixel_shader.cso")))
			Quit();

		if (FAILED(ps_wireframe.CreateShader(L"wireframe_pixel.cso")))
			Quit();


		if (FAILED(vbuffer.CreateBuffer(qube_vertices, ARRAYSIZE(qube_vertices))))
			Quit();

		DWORD indices[]
		{
			// front
			0, 1, 2,
			0, 2, 3,

			// back
			7, 6, 5,
			7, 5, 4,

			// left
			4, 5, 1,
			4, 1, 0,

			// right
			3, 2, 6,
			3, 6, 7,

			// up
			1, 5, 6,
			1, 6, 2,

			// down
			4, 0, 3,
			4, 3, 7,
		};
		if (FAILED(ibuffer.CreateBuffer(indices, ARRAYSIZE(indices))))
			Quit();

		if (FAILED(cbuffer.CreateBuffer(DirectX::XMMatrixTranslation(0.5f, 0.2f, 0.0f))))
			Quit();

		camera.aspectRatio = static_cast<float>(this->window.Width()) / this->window.Height();
		camera.UpdateProjectionMatrix();
		camera.position = { -5.0f, 0.0f, -5.0f, 0.0f };

		gfx.DeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	}

	Camera camera;
	float playerSpeed = 2.0f;
	float sensivity = 1.0f;
	bool speedup = false;

	virtual void Update() override
	{
		if (keyboard.GetKey(KEY::ESC).Pressed)
		{
			Quit();
		}

		DirectX::XMVECTOR movement = { 0.0f, 0.0f, 0.0f, 0.0f };

		float speedmult = 1.0f;
		if (keyboard.GetKey(KEY::X).Pressed)
		{
			speedup = !speedup;
		}
		if (speedup)
		{
			speedmult = 20.0f;
		}

		if (keyboard.GetKey(KEY::W).Held)
		{
			movement.m128_f32[2] += playerSpeed * speedmult * DXEngine::DeltaTime();
		}
		if (keyboard.GetKey(KEY::A).Held)
		{
			movement.m128_f32[0] -= playerSpeed * speedmult * DXEngine::DeltaTime();
		}
		if (keyboard.GetKey(KEY::S).Held)
		{
			movement.m128_f32[2] -= playerSpeed * speedmult * DXEngine::DeltaTime();
		}
		if (keyboard.GetKey(KEY::D).Held)
		{
			movement.m128_f32[0] += playerSpeed * speedmult * DXEngine::DeltaTime();
		}
		if (keyboard.GetKey(KEY::SPACE).Held)
		{
			camera.position.m128_f32[1] += playerSpeed * speedmult * DXEngine::DeltaTime();
		}
		if (keyboard.GetKey(KEY::Z).Held)
		{
			camera.position.m128_f32[1] -= playerSpeed * speedmult * DXEngine::DeltaTime();
		}

		camera.ApplyLocalTranslation(movement);

		camera.rotation.m128_f32[1] += mouse.GetDelta().x * DXEngine::DeltaTime() * sensivity;
		camera.rotation.m128_f32[0] += mouse.GetDelta().y * DXEngine::DeltaTime() * sensivity;

		if (keyboard.GetKey(KEY::Q).Held)
		{
			camera.SetFovDegrees(camera.GetFovDegrees() - DXEngine::DeltaTime() * 10.0f);
			camera.UpdateProjectionMatrix();
		}
		if (keyboard.GetKey(KEY::E).Held)
		{
			camera.SetFovDegrees(camera.GetFovDegrees() + DXEngine::DeltaTime() * 10.0f);
			camera.UpdateProjectionMatrix();
		}
		camera.UpdateViewMatrix();
	}

	virtual void Draw() override
	{
		ibuffer.Use();
		vbuffer.Use();
		vs_colorfull.Use();
		ps_colorfull.Use();
		gfx.DeviceContext()->VSSetConstantBuffers(0, 1, cbuffer.GetAddressOf());

		static float time = 0.0f;

		for (int x = 0; x < 100; ++x)
		{
			for (int z = 0; z < 100; ++z)
			{
				//DirectX::XMFLOAT4 color = { (float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX, 1.0f };
				DirectX::XMFLOAT4 color = { 0.2f, 0.2f, 0.2f, 1.0f };
				float time_factor = sin(time + (x + z) / 10.0f);
				for (int i = 0; i < ARRAYSIZE(qube_vertices); ++i)
				{
					qube_vertices[i].color = { time_factor * 0.4f + 0.1f , time_factor * 0.2f + 0.8f, time_factor * 0.2f + 0.8f, 1.0f };
				}
				//float scale_factor = (time_factor + 2.0f) / 1.5f;
				float scale_factor = 2.0f;

				vbuffer.SetVertices(qube_vertices);
				cbuffer.data = DirectX::XMMatrixScaling(scale_factor, scale_factor, scale_factor)
					* DirectX::XMMatrixTranslation(x * 4.0f, time_factor * 4.0f, z * 4.0f) * camera.view * camera.projection;
				cbuffer.data = DirectX::XMMatrixTranspose(cbuffer.data);
				cbuffer.ApplyChanges();
				gfx.DeviceContext()->DrawIndexed(ibuffer.Count(), 0, 0);
			}
		}

		vs_wireframe.Use();
		ps_wireframe.Use();
		gfx.DeviceContext()->VSSetConstantBuffers(0, 1, cbuffer.GetAddressOf());

		DirectX::XMFLOAT4 color = { 0.1f, 0.1f, 0.1f, 1.0f };
		for (int i = 0; i < ARRAYSIZE(qube_vertices); ++i)
		{
			qube_vertices[i].color = color;
		}
		vbuffer.SetVertices(qube_vertices);

		cbuffer.data = DirectX::XMMatrixScaling((sin(time * 3.0f) + 2.0f) * 2.0f, (sin(time * 3.0f + DirectX::XM_2PI / 3.0f) + 2.0f) * 2.0f, (sin(time * 3.0f + DirectX::XM_2PI / 3.0f) + 2.0f) * 2.0f)
			* DirectX::XMMatrixTranslation(-20.0f, 0.0f, -20.0f) * camera.view * camera.projection;

		cbuffer.data = DirectX::XMMatrixTranspose(cbuffer.data);
		cbuffer.ApplyChanges();
		gfx.DeviceContext()->DrawIndexed(ibuffer.Count(), 0, 0);

		time += DeltaTime() * 0.5f;
	}
};


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{

	try
	{
		HRESULT hr = CoInitialize(NULL);
		if (FAILED(hr))
		{
			ErrorLogger::Log("Failed to call CoInitialise.");
			return -1;
		}

		DemoWindow engine;
		if (engine.InitialiseWindow(hInstance, L"Test", { 1920, 1080 }))
		{
			engine.Start();
		}
	}
	catch (std::exception& exc)
	{
		ErrorLogger::Log(exc.what());
	}


	return 0;
}