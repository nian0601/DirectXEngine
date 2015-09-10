#include "stdafx.h"
#include "Texture.h"
#include <DL_Debug.h>

Texture::Texture()
{
}


Texture::~Texture()
{
}

bool Texture::LoadTexture(const std::string& aFilePath)
{
	HRESULT hr = D3DX11CreateShaderResourceViewFromFile(Engine::GetInstance()->GetDevice(), aFilePath.c_str(), NULL, NULL, &myTexture, NULL);
	myFileName = aFilePath;
	if (FAILED(hr) != S_OK)
	{
		RESOURCE_LOG("Failed to load: %s, trying to use debug-texture.", aFilePath.c_str());
		hr = D3DX11CreateShaderResourceViewFromFile(Engine::GetInstance()->GetDevice(), "Data/resources/texture/uv_map.png", NULL, NULL, &myTexture, NULL);
		myFileName = "Data/resources/texture/uv_map.png";
		if (FAILED(hr) != S_OK)
		{
			DL_ASSERT("[Texture]: Failed to load debug-texture: Data/resources/texture/uv_map.png");
		}
	}

	return true;
}

const std::string& Texture::GetFileName() const
{
	return myFileName;
}

ID3D11ShaderResourceView* Texture::GetShaderView()
{
	return myTexture;
}