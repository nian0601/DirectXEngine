#include "stdafx.h"
#include "Camera.h"
#include <xnamath.h>

Camera::Camera()
{
	OnResize(800, 600);

	myOrientation.SetPos(CU::Vector3<float>(0.f, 0.f, -25.f));
}


Camera::~Camera()
{
}

void Camera::OnResize(const int aWidth, const int aHeight)
{
	myProjectionMatrix = CU::Matrix44<float>::CreateProjectionMatrixLH(0.1f, 1000.f, static_cast<float>(aWidth / aHeight), XM_PI * 0.5f);
}

CU::Matrix44<float>& Camera::GetOrientation()
{
	return myOrientation;
}

CU::Vector3<float>& Camera::GetPosition()
{
	return myPosition;
}

CU::Matrix44<float>& Camera::GetProjection()
{
	return myProjectionMatrix;
}

void Camera::SetOrientation(const CU::Matrix44<float>& aOrientation)
{
	myOrientation = aOrientation;
}

void Camera::SetPosition(const CU::Vector3<float>& aPosition)
{
	myPosition = aPosition;
	myOrientation.SetPos(aPosition);
}

void Camera::RotateX(const float aDegrees)
{
	myPosition = myOrientation.GetPos();
	myOrientation.SetPos({ 0.f, 0.f, 0.f, 0.f });
	myOrientation = CU::Matrix44<float>::CreateRotateAroundX(aDegrees * XM_PI / 180.f) * myOrientation;
	myOrientation.SetPos(myPosition);
}

void Camera::RotateY(const float aDegrees)
{
	myPosition = myOrientation.GetPos();
	myOrientation.SetPos({ 0.f, 0.f, 0.f, 0.f });
	myOrientation = CU::Matrix44<float>::CreateRotateAroundY(aDegrees * XM_PI / 180.f) * myOrientation;
	myOrientation.SetPos(myPosition);
}

void Camera::RotateZ(const float aDegrees)
{
	myPosition = myOrientation.GetPos();
	myOrientation.SetPos({ 0.f, 0.f, 0.f, 0.f });
	myOrientation = CU::Matrix44<float>::CreateRotateAroundZ(aDegrees * XM_PI / 180.f) * myOrientation;
	myOrientation.SetPos(myPosition);
}

void Camera::MoveForward(const float aDistance)
{
	myPosition += myOrientation.GetForward() * aDistance;
	myOrientation.SetPos(myPosition);
}

void Camera::MoveRight(const float aDistance)
{
	myPosition += myOrientation.GetRight() * aDistance;
	myOrientation.SetPos(myPosition);
}