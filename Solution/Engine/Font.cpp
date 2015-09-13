#include "stdafx.h"
#include "Font.h"


Font::Font()
	: myCharSize(32.f, 32.f)
{
}


Font::~Font()
{
}

bool Font::Init(const char* aFontPath)
{
	myTexture = Engine::GetInstance()->GetTextureContainer().GetTexture(aFontPath);

	return true;
}

Font::CharacterData Font::GetCharData(char aChar)
{
	int x = aChar % 16;
	int y = aChar / 16;

	
	CU::Vector2<float> topLeft(x * myCharSize.x, y * myCharSize.y);
	CU::Vector2<float> bottomLeft(topLeft.x + myCharSize.x, topLeft.y + myCharSize.y);


	CharacterData data;
	data.myTopLeftUV = topLeft / 512.f;
	data.myBottomRightUV = bottomLeft / 512.f;

	return data;
}