#include "stdafx.h"
#include "Text.h"
#include "Font.h"
#include "Engine.h"
#include "Effect.h"
#include <string.h>
#include "VertexBufferWrapper.h"
#include "IndexBufferWrapper.h"
#include "Surface.h"
#include "Camera.h"

Text::Text()
{
}


Text::~Text()
{
}

void Text::Init(Font* aFont)
{
	myEffect = Engine::GetInstance()->GetEffectContainer().GetEffect("Data/effect/FontEffect.fx");
	myFont = aFont;

	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3DX11_PASS_DESC passDesc;
	myEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);
	HRESULT hr = Engine::GetInstance()->GetDevice()->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &myVertexLayout);
	if (FAILED(hr) != S_OK)
	{
		DL_MESSAGE_BOX("Failed to CreateInputLayout", "Text::Init", MB_ICONWARNING);
	}

	Engine::GetInstance()->GetContex()->IASetInputLayout(myVertexLayout);

	myVertices.Init(6);
	myVerticeIndices.Init(6);
	myVertexBuffer = nullptr;
	myIndexBuffer = nullptr;
	mySurface = new Surface();
	UpdateSentence("   ", 0.f, 0.f);

	InitSentence();
	InitIndexBuffer();

	mySurface->SetEffect(myEffect);
	mySurface->SetIndexCount(myVerticeIndices.Size());
	mySurface->SetIndexStart(0);
	mySurface->SetVertexCount(myVertices.Size());
	mySurface->SetVertexStart(0);
	mySurface->SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mySurface->SetTexture("DiffuseTexture", myFont->GetTexture());
}

void Text::Render(Camera& aCamera)
{
	myEffect->SetViewMatrix(CU::InverseSimple(aCamera.GetOrientation()));
	myEffect->SetProjectionMatrix(aCamera.GetProjection());
	myEffect->SetWorldMatrix(myOrientation);

	Engine::GetInstance()->GetContex()->IASetInputLayout(myVertexLayout);


	Engine::GetInstance()->GetContex()->IASetInputLayout(myVertexLayout);
	Engine::GetInstance()->GetContex()->IASetVertexBuffers(myVertexBuffer->myStartSlot, myVertexBuffer->myNumberOfBuffers, &myVertexBuffer->myVertexBuffer, &myVertexBuffer->myStride, &myVertexBuffer->myByteOffset);
	Engine::GetInstance()->GetContex()->IASetIndexBuffer(myIndexBuffer->myIndexBuffer, myIndexBuffer->myIndexBufferFormat, myIndexBuffer->myByteOffset);


	D3DX11_TECHNIQUE_DESC techDesc;
	myEffect->GetTechnique()->GetDesc(&techDesc);

	mySurface->Activate();

	for (UINT i = 0; i < techDesc.Passes; ++i)
	{
		myEffect->GetTechnique()->GetPassByIndex(i)->Apply(0, Engine::GetInstance()->GetContex());
		Engine::GetInstance()->GetContex()->DrawIndexed(mySurface->GetIndexCount(), mySurface->GetVertexStart(), 0);
	}
}

void Text::InitSentence()
{
	delete myVertexBuffer;
	myVertexBuffer = new VertexBufferWrapper();

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VertexPosUV) * myVertices.Size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));

	InitData.pSysMem = reinterpret_cast<char*>(&myVertices[0]);

	HRESULT hr = Engine::GetInstance()->GetDevice()->CreateBuffer(&bd, &InitData, &myVertexBuffer->myVertexBuffer);
	if (FAILED(hr) != S_OK)
	{
		int apa = 5;
		++apa;
	}

	myVertexBuffer->myStride = sizeof(VertexPosUV);
	myVertexBuffer->myByteOffset = 0;
	myVertexBuffer->myStartSlot = 0;
	myVertexBuffer->myNumberOfBuffers = 1;
}

void Text::InitVertexBuffer()
{
	
}

void Text::InitIndexBuffer()
{
	delete myIndexBuffer;
	myIndexBuffer = new IndexBufferWrapper();
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(UINT) * myVerticeIndices.Size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	bd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));

	InitData.pSysMem = reinterpret_cast<char*>(&myVerticeIndices[0]);

	HRESULT hr = Engine::GetInstance()->GetDevice()->CreateBuffer(&bd, &InitData, &myIndexBuffer->myIndexBuffer);
	if (FAILED(hr) != S_OK)
	{
		int apa = 5;
		++apa;
	}

	myIndexBuffer->myIndexBufferFormat = DXGI_FORMAT_R32_UINT;
	myIndexBuffer->myByteOffset = 0;
}

void Text::UpdateSentence(char* aString, const float aDrawX, const float aDrawY)
{
	int numOfLetters = static_cast<int>(strlen(aString));
	int index = 0;
	float drawX = aDrawX;
	float drawY = aDrawY;
	CU::Vector2<float> charSize = myFont->GetCharSize();

	myVertices.RemoveAll();
	myVerticeIndices.RemoveAll();
	for (int i = 0; i < numOfLetters; ++i)
	{
		Font::CharacterData charData = myFont->GetCharData(aString[i]);

		//First Triangle

		//TopLeft
		VertexPosUV vert;
		vert.myPos = CU::Vector3<float>(drawX, drawY, 0.f);
		vert.myUV = CU::Vector2<float>(charData.myTopLeftUV);
		myVertices.Add(vert);
		myVerticeIndices.Add(index);
		++index;

		//BottomRight
		vert.myPos = CU::Vector3<float>(drawX + charSize.x, drawY - 16.f, 0.f);
		vert.myUV = CU::Vector2<float>(charData.myBottomRightUV);
		myVertices.Add(vert);
		myVerticeIndices.Add(index);
		++index;


		//BottomLeft
		vert.myPos = CU::Vector3<float>(drawX, drawY - 16.f, 0.f);
		vert.myUV = CU::Vector2<float>(charData.myTopLeftUV.x, charData.myBottomRightUV.y);
		myVertices.Add(vert);
		myVerticeIndices.Add(index);
		++index;

		//Second Triangle

		//TopLeft
		vert.myPos = CU::Vector3<float>(drawX, drawY, 0.f);
		vert.myUV = CU::Vector2<float>(charData.myTopLeftUV);
		myVertices.Add(vert);
		myVerticeIndices.Add(index);
		++index;

		//TopRight
		vert.myPos = CU::Vector3<float>(drawX + charSize.x, drawY, 0.f);
		vert.myUV = CU::Vector2<float>(charData.myBottomRightUV.x, charData.myTopLeftUV.y);
		myVertices.Add(vert);
		myVerticeIndices.Add(index);
		++index;

		//BottomRight
		vert.myPos = CU::Vector3<float>(drawX + charSize.x, drawY - 16.f, 0.f);
		vert.myUV = CU::Vector2<float>(charData.myBottomRightUV);
		myVertices.Add(vert);
		myVerticeIndices.Add(index);
		++index;

		drawX += charSize.x + 1.f;
	}

	InitSentence();
	InitIndexBuffer();

	mySurface->SetIndexCount(myVerticeIndices.Size());
	mySurface->SetVertexCount(myVertices.Size());
}