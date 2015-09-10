#include "stdafx.h"
#include "Game.h"

#include <Instance.h>
#include <Scene.h>
#include <Light.h>
#include <TimerManager.h>

#include <GeometryGenerator.h>

Game::Game()
{

}
Game::~Game()
{
	delete myScene;
	myInstances.DeleteAll();
}

bool Game::Init()
{
	myLight = new Light();
	myLight->SetLightColor({ 0.3f, 0.3f, 0.3f, 1.f });
	myLight->SetLightDir({ 0.f, 0.5f, -1.f });
	myInstances.Init(4);

	myWaveModel = Engine::GetInstance()->LoadModel("Data/resources/model/companion/companion.fbx", Engine::GetInstance()->GetEffectContainer().GetEffect("Data/effect/Wave.fx"));
	myGravityModel = Engine::GetInstance()->LoadModel("Data/resources/model/companion/companion.fbx", Engine::GetInstance()->GetEffectContainer().GetEffect("Data/effect/GravityWell.fx"));
	myExtrudeModel = Engine::GetInstance()->LoadModel("Data/resources/model/companion/companion.fbx", Engine::GetInstance()->GetEffectContainer().GetEffect("Data/effect/Extrude.fx"));
	myNormalModel = Engine::GetInstance()->LoadModel("Data/resources/model/companion/companion.fbx", Engine::GetInstance()->GetEffectContainer().GetEffect("Data/effect/BasicEffect.fx"));

	myInstances.Add(new Instance(*myWaveModel));
	myInstances.Add(new Instance(*myGravityModel));
	myInstances.Add(new Instance(*myExtrudeModel));
	myInstances.Add(new Instance(*myNormalModel));

	for (int i = 0; i < 1000; ++i)
		myInstances.Add(new Instance(*myNormalModel));

	myInstances[0]->SetPosition({ -15.f, 0.f, 0.f });
	myInstances[2]->SetPosition({ 15.f, 0.f, 0.f });
	myInstances[3]->SetPosition({ 0.f, 15.f, 0.f });

	//MeshData worldMesh;
	//GeometryGenerator::CreateGrid(500.f, 500.f, 100, 100, worldMesh);
	//myGeometryModel = new Model();
	//myGeometryModel->InitGeometry(Engine::GetInstance()->GetEffectContainer().GetEffect("Data/effect/BasicEffect.fx"), worldMesh);
	//myInstances.Add(new Instance(*myGeometryModel));

	myScene = new Scene();
	myScene->SetCamera(&myCamera);
	for (int i = 0; i < myInstances.Size(); ++i)
		myScene->AddInstance(myInstances[i]);

	myScene->AddLight(myLight);

	GAME_LOG("Init Successful");
	return true;
}

bool Game::Destroy()
{
	return true;
}

bool Game::Update()
{
	DL_TIME_FUNCTION
	CU::TimerManager::GetInstance()->Update();
	float deltaTime = CU::TimerManager::GetInstance()->GetMasterTimer().GetTime().GetFrameTime();

	Engine::GetInstance()->GetEffectContainer().Update(deltaTime);

	myLight->Update();
	myLight->PerformRotation(CU::Matrix33<float>::CreateRotateAroundY(-0.005f * 3.14f / 180.f));
	//myInstance->PerformRotationWorld(CU::Matrix44<float>::CreateRotateAroundY(0.005f * 3.14f / 180.f));		
	//myInstance->PerformRotationWorld(CU::Matrix44<float>::CreateRotateAroundZ(0.005f * 3.14f / 180.f));

	myInstances[3]->PerformRotationLocal(CU::Matrix44<float>::CreateRotateAroundX((720 * deltaTime) * 3.14f / 180.f));
	myInstances[3]->PerformRotationLocal(CU::Matrix44<float>::CreateRotateAroundY((720 * deltaTime) * 3.14f / 180.f));

	myScene->Render();

	return true;
}

void Game::Pause()
{

}
void Game::UnPause()
{

}
void Game::OnResize(int aWidth, int aHeight)
{
	myCamera.OnResize(aWidth, aHeight);
}

void Game::LogicUpdate()
{
	
}
