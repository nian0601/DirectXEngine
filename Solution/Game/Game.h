#pragma once
#include "stdafx.h"
#include <Model.h>
#include <Camera.h>

class Scene;
class Instance;
class Light;
class Game
{
public:
	Game();
	~Game();

	bool Init();
	bool Destroy();
	bool Update();

	void Pause();
	void UnPause();
	void OnResize(int aWidth, int aHeight);

	void LogicUpdate();

private:
	void operator=(Game& aApp) = delete;

	Model* myWaveModel;
	Model* myGravityModel;
	Model* myExtrudeModel;
	Model* myNormalModel;
	Model* myGeometryModel;
	CU::GrowingArray<Instance*> myInstances;
	Scene* myScene;
	Camera myCamera;
	Light* myLight;
	CU::Matrix44<float> myWorldMatrix;
};

