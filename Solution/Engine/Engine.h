#pragma once
#include "stdafx.h"
#include "TextureContainer.h"
#include "EffectContainer.h"
#include "FBXFactory.h"

class DirectX;
struct SetupInfo;

class Engine
{
public:
	static bool Create(HWND& aHwnd, WNDPROC aWndProc, SetupInfo& aSetupInfo);
	static Engine* GetInstance();
	void Shutdown();
	void SwitchBuffers();
	void OnResize(int aWidth, int aHeigth);

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContex();
	TextureContainer& GetTextureContainer();
	EffectContainer& GetEffectContainer();
	Model* LoadModel(const std::string& aPath, Effect* aEffect);

	void BeginScene();
	void EndScene();

private:
	bool Init(HWND& aHwnd, WNDPROC aWndProc);
	bool WindowSetup(HWND& aHwnd, WNDPROC aWindowProc);
	

	DirectX* myDirectX;
	SetupInfo* mySetupInfo;
	TextureContainer myTextureContainer;
	EffectContainer myEffectContainer;
	FBXFactory myModelFactory;

	static Engine* myInstance;
};

inline ID3D11Device* Engine::GetDevice()
{
	return myDirectX->GetDevice();
}

inline ID3D11DeviceContext* Engine::GetContex()
{
	return myDirectX->GetContex();
}

inline TextureContainer& Engine::GetTextureContainer()
{
	return myTextureContainer;
}

inline EffectContainer& Engine::GetEffectContainer()
{
	return myEffectContainer;
}