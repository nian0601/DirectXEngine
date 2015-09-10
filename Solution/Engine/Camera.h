#pragma once
#include <Matrix44.h>

class Camera
{
public:
	Camera();
	~Camera();

	void OnResize(const int aWidth, const int aHeight);

	CU::Matrix44<float>& GetOrientation();
	CU::Vector3<float>& GetPosition();
	CU::Matrix44<float>& GetProjection();

	void SetOrientation(const CU::Matrix44<float>& aOrientation);
	void SetPosition(const CU::Vector3<float>& aPosition);

private:
	CU::Matrix44<float> myOrientation;
	CU::Matrix44<float> myProjectionMatrix;
	CU::Vector3<float> myPosition;
};

