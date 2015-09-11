#pragma once
#include <GrowingArray.h>
#include <Matrix.h>
#include "Vertices.h"

class Effect;
class Font;
class Surface;
class Camera;
struct ID3D11InputLayout;
struct VertexBufferWrapper;
struct IndexBufferWrapper;



class Text
{
public:
	Text();
	~Text();

	void Init(Font* aFont);

	void Render(Camera& aCamera);

	void UpdateSentence(char* aString, const float aDrawX, const float aDrawY);
private:

	void InitSentence();
	void InitVertexBuffer();
	void InitIndexBuffer();


	Font* myFont;
	Effect* myEffect;
	ID3D11InputLayout* myVertexLayout;
	CU::GrowingArray<VertexPosUV> myVertices;
	CU::GrowingArray<int> myVerticeIndices;

	IndexBufferWrapper* myIndexBuffer;
	VertexBufferWrapper* myVertexBuffer;
	Surface* mySurface;

	CU::Matrix44<float> myOrientation;
	CU::Matrix44<float>  myViewMatrix;
	ID3D11BlendState* myBlendState;
};

