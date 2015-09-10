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
	myProjectionMatrix = CU::Matrix44<float>::CreateProjectionMatrixLH(0.1f, 100.f, static_cast<float>(aWidth / aHeight), XM_PI * 0.5f);
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