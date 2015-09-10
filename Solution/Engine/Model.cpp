#include "stdafx.h"
#include "Model.h"
#include "Effect.h"
#include "Engine.h"
#include "Surface.h"
#include "GeometryGenerator.h"

#include "IndexBufferWrapper.h"
#include "VertexBufferWrapper.h"
#include "VertexDataWrapper.h"
#include "VertexIndexWrapper.h"

Model::Model()
{
	myChilds.Init(2);
	myChildTransforms.Init(2);
	myVertices.Init(64);
	myVerticeIndices.Init(64);
	mySurfaces.Init(4);
	myVertexFormat.Init(2);
	myIsNULLObject = true;
}


Model::~Model()
{
}

void Model::Init()
{
	if (myIsNULLObject == false)
	{
		const int size = myVertexFormat.Size();
		D3D11_INPUT_ELEMENT_DESC* vertexDesc = new D3D11_INPUT_ELEMENT_DESC[size];
		for (int i = 0; i < myVertexFormat.Size(); ++i)
		{
			vertexDesc[i] = *myVertexFormat[i];
		}


		D3DX11_PASS_DESC passDesc;
		HRESULT hr = S_OK;
		hr = myEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);
		hr = Engine::GetInstance()->GetDevice()->CreateInputLayout(vertexDesc, size, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &myVertexLayout);

		Engine::GetInstance()->GetContex()->IASetInputLayout(myVertexLayout);

		InitVertexBuffer();
		InitIndexBuffer();
	}

	for (int i = 0; i < myChilds.Size(); ++i)
	{
		myChilds[i]->Init();
	}
}

void Model::InitPolygon(Effect* aEffect)
{
	myEffect = aEffect;

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3DX11_PASS_DESC passDesc;
	myEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);
	Engine::GetInstance()->GetDevice()->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &myVertexLayout);
	
	Engine::GetInstance()->GetContex()->IASetInputLayout(myVertexLayout);


	//myVertices.Add({ { 0.0f, 0.5f, 0.5f }, { 1.f, 0.f, 0.f, 1.f } });
	//myVertices.Add({ { 0.5f, -0.5f, 0.5 }, { 0.f, 1.f, 0.f, 1.f } });
	//myVertices.Add({ { -0.5f, -0.5f, 0.5f }, { 0.f, 0.f, 1.f, 1.f } });


	myVerticeIndices.Add(0);
	myVerticeIndices.Add(1);
	myVerticeIndices.Add(2);

	InitBuffers(VertexType::POS_NORM_UV);
}

void Model::InitCube(Effect* aEffect)
{
	myEffect = aEffect;

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3DX11_PASS_DESC passDesc;
	myEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);
	Engine::GetInstance()->GetDevice()->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &myVertexLayout);

	Engine::GetInstance()->GetContex()->IASetInputLayout(myVertexLayout);

	//0 - 3 (Top)
	myVertices.Add({ { -1.0f, 1.0f, -1.0f }, { 0.f, 1.f, 0.f }, {0.0f, 0.0f} });
	myVertices.Add({ { 1.0f, 1.0f, -1.0f }, { 0.f, 1.f, 0.f }, { 1.0f, 0.0f } });
	myVertices.Add({ { 1.0f, 1.0f, 1.0f }, { 0.f, 1.f, 0.f }, { 1.0f, 1.0f } });
	myVertices.Add({ { -1.0f, 1.0f, 1.0f }, { 0.f, 1.f, 0.f }, { 0.0f, 1.0f } });

	//4 - 7 (Bottom)
	myVertices.Add({ { -1.0f, -1.0f, -1.0f }, { 0.f, -1.f, 0.f }, { 0.0f, 0.0f } });
	myVertices.Add({ { 1.0f, -1.0f, -1.0f }, { 0.f, -1.f, 0.f }, { 1.0f, 0.0f } });
	myVertices.Add({ { 1.0f, -1.0f, 1.0f }, { 0.f, -1.f, 0.f }, { 1.0f, 1.0f } });
	myVertices.Add({ { -1.0f, -1.0f, 1.0f }, { 0.f, -1.f, 0.f }, { 0.0f, 1.0f } });

	//8 - 11 (Left)
	myVertices.Add({ { -1.0f, -1.0f, 1.0f }, { -1.f, 0.f, 0.f }, { 0.0f, 0.0f } });
	myVertices.Add({ { -1.0f, -1.0f, -1.0f }, { -1.f, 0.f, 0.f }, { 1.0f, 0.0f } });
	myVertices.Add({ { -1.0f, 1.0f, -1.0f }, { -1.f, 0.f, 0.f }, { 1.0f, 1.0f } });
	myVertices.Add({ { -1.0f, 1.0f, 1.0f }, { -1.f, 0.f, 0.f }, { 0.0f, 1.0f } });
 
	//12 - 15 (Right)
	myVertices.Add({ { 1.0f, -1.0f, 1.0f }, { 1.f, 0.f, 0.f }, { 0.0f, 0.0f } });
	myVertices.Add({ { 1.0f, -1.0f, -1.0f }, { 1.f, 0.f, 0.f }, { 1.0f, 0.0f } });
	myVertices.Add({ { 1.0f, 1.0f, -1.0f }, { 1.f, 0.f, 0.f }, { 1.0f, 1.0f } });
	myVertices.Add({ { 1.0f, 1.0f, 1.0f }, { 1.f, 0.f, 0.f }, { 0.0f, 1.0f } });

	//16 - 19 (Front)
	myVertices.Add({ { -1.0f, -1.0f, -1.0f }, { 0.f, 0.f, -1.f }, { 0.0f, 0.0f } });
	myVertices.Add({ { 1.0f, -1.0f, -1.0f }, { 0.f, 0.f, -1.f }, { 1.0f, 0.0f } });
	myVertices.Add({ { 1.0f, 1.0f, -1.0f }, { 0.f, 0.f, -1.f }, { 1.0f, 1.0f } });
	myVertices.Add({ { -1.0f, 1.0f, -1.0f }, { 0.f, 0.f, -1.f }, { 0.0f, 1.0f } });
 
	//20 - 23 (Back)
	myVertices.Add({ { -1.0f, -1.0f, 1.0f }, { 0.f, 0.f, 1.f }, { 0.0f, 0.0f } });
	myVertices.Add({ { 1.0f, -1.0f, 1.0f }, { 0.f, 0.f, 1.f }, { 1.0f, 0.0f } });
	myVertices.Add({ { 1.0f, 1.0f, 1.0f }, { 0.f, 0.f, 1.f }, { 1.0f, 1.0f } });
	myVertices.Add({ { -1.0f, 1.0f, 1.0f }, { 0.f, 0.f, 1.f }, { 0.0f, 1.0f } });

#pragma region Indices

	//Top
	myVerticeIndices.Add(3);
	myVerticeIndices.Add(1);
	myVerticeIndices.Add(0);
				   
	myVerticeIndices.Add(2);
	myVerticeIndices.Add(1);
	myVerticeIndices.Add(3);
				   
	//Bottom
	myVerticeIndices.Add(6);
	myVerticeIndices.Add(4);
	myVerticeIndices.Add(5);
				   
	myVerticeIndices.Add(7);
	myVerticeIndices.Add(4);
	myVerticeIndices.Add(6);
				   
	//Left
	myVerticeIndices.Add(11);
	myVerticeIndices.Add(9);
	myVerticeIndices.Add(8);
				   
	myVerticeIndices.Add(10);
	myVerticeIndices.Add(9);
	myVerticeIndices.Add(11);
				   
	//Right
	myVerticeIndices.Add(14);
	myVerticeIndices.Add(12);
	myVerticeIndices.Add(13);
				   
	myVerticeIndices.Add(15);
	myVerticeIndices.Add(12);
	myVerticeIndices.Add(14);
				   
	//Front
	myVerticeIndices.Add(19);
	myVerticeIndices.Add(17);
	myVerticeIndices.Add(16);
				   
	myVerticeIndices.Add(18);
	myVerticeIndices.Add(17);
	myVerticeIndices.Add(19);
	
	//Back
	myVerticeIndices.Add(22);
	myVerticeIndices.Add(20);
	myVerticeIndices.Add(21);
				   
	myVerticeIndices.Add(23);
	myVerticeIndices.Add(20);
	myVerticeIndices.Add(22);
				   
#pragma endregion
	

	InitBuffers(VertexType::POS_NORM_UV);

	Surface surf;
	surf.SetEffect(myEffect);
	surf.SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	surf.SetVertexStart(0);
	surf.SetVertexCount(myVertices.Size());
	surf.SetIndexStart(0);
	surf.SetIndexCount(myVerticeIndices.Size());
	
	surf.SetTexture("DiffuseTexture", "Data/resources/texture/seafloor.dds", true);

	mySurfaces.Add(surf);
}

void Model::InitGeometry(Effect* aEffect, const MeshData& aMeshData)
{
	//myEffect = aEffect;
	//
	//D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	//{
	//	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	//};
	//
	//D3DX11_PASS_DESC passDesc;
	//myEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);
	//Engine::GetInstance()->GetDevice()->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &myVertexLayout);
	//
	//Engine::GetInstance()->GetContex()->IASetInputLayout(myVertexLayout);
	//
	//myVertices.Init(aMeshData.myVertices.Size());
	//for (int i = 0; i < aMeshData.myVertices.Size(); ++i)
	//{
	//	VertexPosNormColor vertex;
	//	vertex.myPos = aMeshData.myVertices[i].myPosition;
	//	vertex.myNorm = aMeshData.myVertices[i].myNormal;
	//	vertex.myColor = aMeshData.myVertices[i].myColor;
	//
	//	myVertices.Add(vertex);
	//}
	//
	//myVerticeIndices.Init(aMeshData.myIndices.Size());
	//for (int i = 0; i < aMeshData.myIndices.Size(); ++i)
	//{
	//	myVerticeIndices.Add(aMeshData.myIndices[i]);
	//}
	//
	//InitBuffers(VertexType::POS_NORM_COLOR);
}

void Model::AddChild(Model* aChild)
{
	myChilds.Add(aChild);
	myChildTransforms.Add(aChild->myOrientation);
}

Effect* Model::GetEffect()
{
	return myEffect;
}

void Model::Render(const CU::Matrix44<float>& aOrientation)
{
	if (myIsNULLObject == false)
	{
		myEffect->SetWorldMatrix(aOrientation);

		Engine::GetInstance()->GetContex()->IASetVertexBuffers(myVertexBuffer->myStartSlot, myVertexBuffer->myNumberOfBuffers, &myVertexBuffer->myVertexBuffer, &myVertexBuffer->myStride, &myVertexBuffer->myByteOffset);
		Engine::GetInstance()->GetContex()->IASetIndexBuffer(myIndexBuffer->myIndexBuffer, myIndexBuffer->myIndexBufferFormat, myIndexBuffer->myByteOffset);


		D3DX11_TECHNIQUE_DESC techDesc;
		myEffect->GetTechnique()->GetDesc(&techDesc);

		for (int s = 0; s < mySurfaces.Size(); ++s)
		{
			mySurfaces[s].Activate();

			for (UINT i = 0; i < techDesc.Passes; ++i)
			{
				myEffect->GetTechnique()->GetPassByIndex(i)->Apply(0, Engine::GetInstance()->GetContex());
				Engine::GetInstance()->GetContex()->DrawIndexed(mySurfaces[s].GetIndexCount(), mySurfaces[s].GetVertexStart(), 0);
			}
		}
	}

	for (int i = 0; i < myChilds.Size(); ++i)
	{
		myChilds[i]->Render(myChildTransforms[i] * aOrientation);
	}
}

bool Model::InitBuffers(VertexType aVertexType)
{
	myVertexBaseData = new VertexDataWrapper();
	myVertexBaseData->myNumberOfVertices = myVertices.Size();
	myVertexBaseData->myStride = 0;
	myVertexBaseData->myType = aVertexType;
	switch (aVertexType)
	{
	case VertexType::POS_COLOR:
		myVertexBaseData->mySize = sizeof(VertexPosColor);
		break;
	case VertexType::POS_UV:
		myVertexBaseData->mySize = sizeof(VertexPosUV);
		break;
	case VertexType::POS_COLOR_UV:
		myVertexBaseData->mySize = sizeof(VertexPosColorUV);
		break;
	case VertexType::POS_NORM_UV:
		myVertexBaseData->mySize = sizeof(VertexPosNormUV);
		break;
	default:
		DL_ASSERT("[Model]: Invalid input to InitBuffers");
		break;
	}

	myVertexBaseData->mySize *= myVertexBaseData->myNumberOfVertices;
	
	myVertexBaseData->myVertexData = reinterpret_cast<char*>(&myVertices[0]);

	myIndexBaseData = new VertexIndexWrapper();
	myIndexBaseData->myFormat = DXGI_FORMAT_R32_UINT;
	myIndexBaseData->myIndexData = reinterpret_cast<char*>(&myVerticeIndices[0]);
	myIndexBaseData->myNumberOfIndices = myVerticeIndices.Size();
	myIndexBaseData->mySize = sizeof(UINT) * myIndexBaseData->myNumberOfIndices;

	if (InitVertexBuffer() == false)
		return false;

	if (InitIndexBuffer() == false)
		return false;

	return true;
}

bool Model::InitVertexBuffer()
{
	myVertexBuffer = new VertexBufferWrapper();

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = myVertexBaseData->mySize;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = myVertexBaseData->myStride;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));

	InitData.pSysMem = myVertexBaseData->myVertexData;
	
	HRESULT hr = Engine::GetInstance()->GetDevice()->CreateBuffer(&bd, &InitData, &myVertexBuffer->myVertexBuffer);
	if (FAILED(hr) != S_OK)
		return false;

	myVertexBuffer->myStride = myVertexBaseData->myStride;
	myVertexBuffer->myByteOffset = 0;
	myVertexBuffer->myStartSlot = 0;
	myVertexBuffer->myNumberOfBuffers = 1;

	return true;
}

bool Model::InitIndexBuffer()
{
	myIndexBuffer = new IndexBufferWrapper();

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = myIndexBaseData->mySize;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));

	InitData.pSysMem = myIndexBaseData->myIndexData;

	HRESULT hr = Engine::GetInstance()->GetDevice()->CreateBuffer(&bd, &InitData, &myIndexBuffer->myIndexBuffer);
	if (FAILED(hr) != S_OK)
		return false;

	myIndexBuffer->myIndexBufferFormat = myIndexBaseData->myFormat;
	myIndexBuffer->myByteOffset = 0;

	return true;
}
