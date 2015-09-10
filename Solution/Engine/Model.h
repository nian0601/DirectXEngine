#pragma once

#include "Vertices.h"
#include <GrowingArray.h>
#include <Matrix.h>
#include "Surface.h"


class Effect;
struct MeshData;
struct IndexBufferWrapper;
struct VertexBufferWrapper;
struct VertexDataWrapper;
struct VertexIndexWrapper;
struct ID3DX11EffectShaderResourceVariable;

class Model
{
	friend class FBXFactory;
public:
	Model();
	~Model();

	void Init();
	void InitPolygon(Effect* aEffect);
	void InitCube(Effect* aEffect);
	void AddChild(Model* aChild);
	void InitGeometry(Effect* aEffect, const MeshData& aMeshData);

	Effect* GetEffect();

	void Render(const CU::Matrix44<float>& aOrientation);

private:
	bool InitBuffers(VertexType aVertexType);
	bool InitVertexBuffer();
	bool InitIndexBuffer();

	Effect* myEffect;
	ID3D11InputLayout* myVertexLayout;
	CU::GrowingArray<VertexPosNormUV> myVertices;
	CU::GrowingArray<int> myVerticeIndices;
	CU::GrowingArray<D3D11_INPUT_ELEMENT_DESC*> myVertexFormat;
	bool myIsNULLObject;

	VertexIndexWrapper* myIndexBaseData;
	IndexBufferWrapper* myIndexBuffer;
	VertexDataWrapper* myVertexBaseData;
	VertexBufferWrapper* myVertexBuffer;

	CU::GrowingArray<Surface> mySurfaces;
	CU::GrowingArray<Model*> myChilds;
	CU::GrowingArray<CU::Matrix44f> myChildTransforms;
	CU::Matrix44f myOrientation;
};

