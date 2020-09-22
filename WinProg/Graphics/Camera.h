#pragma once
#include <DirectXMath.h>

class Camera
{
public:
	float aspectRatio = 1.0f;
	float nearZ = 0.1f;
	float farZ = 1000.0f;
	DirectX::XMVECTOR position = { 0.0f, 0.0f, 0.0f, 0.0f };
	DirectX::XMVECTOR rotation = { 0.0f, 0.0f, 0.0f, 0.0f };
	DirectX::XMMATRIX view = DirectX::XMMatrixIdentity();
	DirectX::XMMATRIX projection = DirectX::XMMatrixIdentity();

	void ApplyLocalTranslation(DirectX::XMVECTOR offset);

	void SetFovDegrees(float fovDegrees);
	void SetFovRadians(float fovRadians);
	float GetFovDegrees() const;
	float GetFovRadians() const;
	void UpdateViewMatrix();
	void UpdateProjectionMatrix();
	
private:
	float fovDegrees = 90.0f;
	float fovRadians = DirectX::XM_PIDIV2;

	const DirectX::XMVECTOR DEFAULT_UP_DIRECTION = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	const DirectX::XMVECTOR DEFAULT_FORWARD_DIRECTION = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
};