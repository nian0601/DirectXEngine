#include "stdafx.h"
#include "FBXFactory.h"

#include "Model.h"
#include "FBX/FbxLoader.h"
#include "Matrix44.h"

#include "IndexBufferWrapper.h"
#include "VertexBufferWrapper.h"
#include "VertexDataWrapper.h"
#include "VertexIndexWrapper.h"


FBXFactory::FBXFactory()
{
	CU::Matrix44f test;
	test.SetPos(CU::Vector3f(1, 1, 1));
	CU::Matrix44f resultMatrix;

	myLoader = new FBXLoader();
}

FBXFactory::~FBXFactory()
{

}

	
void FBXFactory::FillData(ModelData* someData, Model* outData, Effect* aEffect)
{
	VertexIndexWrapper* indexWrapper = new VertexIndexWrapper();
	indexWrapper->myFormat = DXGI_FORMAT_R32_UINT;
	unsigned int* indexData = new unsigned int[someData->myIndexCount];
	memcpy(indexData, someData->myIndicies, someData->myIndexCount*sizeof(unsigned int));
	indexWrapper->myIndexData = (char*)indexData;
	indexWrapper->mySize = someData->myIndexCount*sizeof(unsigned int);
	outData->myIndexBaseData = indexWrapper;

	VertexDataWrapper* vertexData = new VertexDataWrapper();
	someData->myVertexBuffer;
	int sizeOfBuffer = someData->myVertexCount*someData->myVertexStride*sizeof(float);
	char* vertexRawData = new char[sizeOfBuffer];
	memcpy(vertexRawData, someData->myVertexBuffer, sizeOfBuffer);
	vertexData->myVertexData = vertexRawData;
	vertexData->myNumberOfVertices = someData->myVertexCount;
	vertexData->mySize = sizeOfBuffer;
	vertexData->myStride = someData->myVertexStride*sizeof(float);

	outData->myVertexBaseData = vertexData;

	for (int i = 0; i < someData->myLayout.Size(); ++i)
	{
		auto currentLayout = someData->myLayout[i];
		D3D11_INPUT_ELEMENT_DESC* desc = new D3D11_INPUT_ELEMENT_DESC();
		desc->SemanticIndex = 0;
		desc->AlignedByteOffset = currentLayout.myOffset;
		desc->InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		desc->InputSlot = 0;
		desc->InstanceDataStepRate = 0;

		if (currentLayout.myType == ModelData::VERTEX_POS)
		{
			desc->SemanticName = "POSITION";
			desc->Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (currentLayout.myType == ModelData::VERTEX_NORMAL)
		{
			desc->SemanticName = "NORMAL";
			desc->Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (currentLayout.myType == ModelData::VERTEX_UV)
		{
			desc->SemanticName = "TEXCOORD";
			desc->Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (currentLayout.myType == ModelData::VERTEX_BINORMAL)
		{
			desc->SemanticName = "BINORMAL";
			desc->Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (currentLayout.myType == ModelData::VERTEX_TANGENT)
		{
			desc->SemanticName = "TANGENT";
			desc->Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (currentLayout.myType == ModelData::VERTEX_SKINWEIGHTS)
		{
			break;
			desc->SemanticName = "WEIGHTS";
			desc->Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
		else if (currentLayout.myType == ModelData::VERTEX_BONEID)
		{
			break;
			desc->SemanticName = "BONES";
			desc->Format = DXGI_FORMAT_R32G32B32A32_SINT;
		}
		outData->myVertexFormat.Add(desc);
	}
	outData->myIsNULLObject = false;
		
	Surface surface;
	surface.SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	surface.SetIndexCount(someData->myIndexCount);
	surface.SetVertexStart(0);
	surface.SetIndexStart(0);
	surface.SetVertexCount(someData->myVertexCount);
	surface.SetEffect(aEffect);
	bool useSRGB = true;
	for (unsigned int i = 0; i < someData->myTextures.size(); ++i)
	{
		auto& currentTexture = someData->myTextures[i];
			
		std::string resourceName;
		if (currentTexture.myType == DIFFUSE)
		{
			resourceName = "DiffuseTexture";
		}
		if (currentTexture.myType == NORMALMAP)
		{
			resourceName = "NormalTexture";
			useSRGB = false;
		}
		if (currentTexture.myType == ROUGHNESS)
		{
			resourceName = "RoughnessTexture";
		}
		if (currentTexture.myType == SUBSTANCE)
		{
			resourceName = "SubstanceTexture";
		}
		if (currentTexture.myType == AO)
		{
			resourceName = "AOTexture";
		}
		surface.SetTexture(resourceName, currentTexture.myFileName, useSRGB);
	}
	outData->mySurfaces.Add(surface);
}

Model* FBXFactory::CreateModel(FbxModelData* someModelData, Effect* aEffect)
{
	Model* tempModel = new Model();
	tempModel->myEffect = aEffect;
		
	if (someModelData->myData)
	{
		FillData(someModelData->myData, tempModel, aEffect);
		tempModel->myOrientation = someModelData->myOrientation;

	}
	for (int i = 0; i < someModelData->myChilds.Size(); ++i)
	{
		auto currentChild = someModelData->myChilds[i];
		tempModel->AddChild(CreateModel(currentChild, aEffect));
	}
	return tempModel;
}

Model* FBXFactory::LoadModel(const char* aFilePath, Effect* aEffect)
{
	FBXData* found = 0;
	for (FBXData* data : myFBXData)
	{ 
		if (data->myPath.compare(aFilePath) == 0)
		{
			found = data;
			break;
		}
	}

	FbxModelData* modelData = nullptr;
	if (found)
	{
		modelData = found->myData;
	}
	else
	{
		FBXData* data = new FBXData();
		FbxModelData* fbxModelData = myLoader->loadModel(aFilePath);
		data->myData = fbxModelData;
		data->myPath = aFilePath;
		myFBXData.push_back(data);
		modelData = data->myData;
	}


	Model* returnModel = CreateModel(modelData, aEffect);
	return returnModel;
}

