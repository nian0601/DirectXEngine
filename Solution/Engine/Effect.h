#pragma once

#include <Matrix.h>
#include <string.h>
#include <d3dx11effect.h>
#include <StaticArray.h>
#include "Texture.h"

class Effect
{
public:
	Effect();
	~Effect();

	ID3DX11Effect* GetEffect();
	ID3DX11EffectTechnique* GetTechnique();
	const std::string& GetFileName() const;
	bool Init(const std::string& aEffectFile);
	void SetWorldMatrix(const CU::Matrix44<float>& aWorldMatrix);
	void SetViewMatrix(const CU::Matrix44<float>& aViewMatrix);
	void SetProjectionMatrix(const CU::Matrix44<float>& aProjectionMatrix);
	void SetBlendState(ID3D11BlendState* aBlendState, float aBlendFactor[4], const unsigned int aSampleMask = 0xFFFFFFFF);
	void UpdateLight(CU::StaticArray<CU::Vector4<float>, 1> someDirs, CU::StaticArray<CU::Vector4<float>, 1> someColors);
	void UpdateTime(const float aDeltaTime);

private:
	ID3DX11Effect* myEffect;
	ID3DX11EffectMatrixVariable* myProjectionMatrixVariable;
	ID3DX11EffectTechnique* myTechnique;
	ID3DX11EffectMatrixVariable* myViewMatrixVariable;
	ID3DX11EffectMatrixVariable* myWorldMatrixVariable;
	ID3DX11EffectVectorVariable* myDirectionalLigthColor;
	ID3DX11EffectVectorVariable* myDirectionalLightDirection;
	ID3DX11EffectScalarVariable* myEyePositionVariable;
	ID3DX11EffectScalarVariable* myTotalTimeVariable;


	std::string myFileName;

};

inline ID3DX11Effect* Effect::GetEffect()
{
	return myEffect;
}

inline ID3DX11EffectTechnique* Effect::GetTechnique()
{
	return myTechnique;
}

inline const std::string& Effect::GetFileName() const
{
	return myFileName;
}

inline void Effect::SetWorldMatrix(const CU::Matrix44<float>& aWorldMatrix)
{
	myWorldMatrixVariable->SetMatrix(&aWorldMatrix.myMatrix[0]);
}

inline void Effect::SetViewMatrix(const CU::Matrix44<float>& aViewMatrix)
{
	myViewMatrixVariable->SetMatrix(&aViewMatrix.myMatrix[0]);
}

inline void Effect::SetProjectionMatrix(const CU::Matrix44<float>& aProjectionMatrix)
{
	myProjectionMatrixVariable->SetMatrix(&aProjectionMatrix.myMatrix[0]);
}

inline void Effect::SetBlendState(ID3D11BlendState* aBlendState, float aBlendFactor[4], const unsigned int aSampleMask)
{
	Engine::GetInstance()->GetContex()->OMSetBlendState(aBlendState, aBlendFactor, aSampleMask);
}