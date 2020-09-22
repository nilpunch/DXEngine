#include "Camera.h"

void Camera::ApplyLocalTranslation(DirectX::XMVECTOR offset)
{
	DirectX::XMVECTOR localOffset = DirectX::XMVector3TransformCoord(offset, DirectX::XMMatrixRotationRollPitchYaw(this->rotation.m128_f32[0], this->rotation.m128_f32[1], this->rotation.m128_f32[2]));
	this->position = DirectX::XMVector3TransformCoord(this->position, DirectX::XMMatrixTranslation(localOffset.m128_f32[0], localOffset.m128_f32[1], localOffset.m128_f32[2]));
}

void Camera::SetFovDegrees(float fovDegrees)
{
	this->fovDegrees = fovDegrees;
	this->fovRadians = (fovDegrees / 360.0f) * DirectX::XM_2PI;
}

void Camera::SetFovRadians(float fovRadians)
{
	this->fovRadians = fovRadians;
	this->fovDegrees = (fovRadians / DirectX::XM_2PI) * 360.0f;
}

float Camera::GetFovDegrees() const
{
	return this->fovDegrees;
}

float Camera::GetFovRadians() const
{
	return this->fovRadians;
}

void Camera::UpdateViewMatrix()
{
	DirectX::XMMATRIX cameraRotation = DirectX::XMMatrixRotationRollPitchYaw(this->rotation.m128_f32[0], this->rotation.m128_f32[1], this->rotation.m128_f32[2]);
	DirectX::XMVECTOR lookingDirection = DirectX::XMVector3TransformCoord(this->DEFAULT_FORWARD_DIRECTION, cameraRotation);
	DirectX::XMVECTOR targetPosition = DirectX::XMVectorAdd(lookingDirection, this->position);
	DirectX::XMVECTOR localUpDirection = DirectX::XMVector3TransformCoord(this->DEFAULT_UP_DIRECTION, cameraRotation);
	this->view = DirectX::XMMatrixLookAtLH(this->position, targetPosition, localUpDirection);
}

void Camera::UpdateProjectionMatrix()
{
	this->projection = DirectX::XMMatrixPerspectiveFovLH(this->fovRadians, this->aspectRatio, this->nearZ, this->farZ);
}
