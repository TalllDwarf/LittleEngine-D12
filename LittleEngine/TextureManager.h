#pragma once
#include <map>
#include "TextureImporter.h"

namespace Texture
{
	static const wchar_t* LOGO = L"Texture/LittleEngine.png";
	static const wchar_t* CAR1V1 = L"Texture/Car_01.png";
	static const wchar_t* CAR2V1 = L"Texture/Car_04.png";
	static const wchar_t* CAR3V1 = L"Texture/Car_07.png";
	static const wchar_t* CAR4V1 = L"Texture/Car_10.png";
}

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	bool Initialise(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);

	std::shared_ptr<TextureImporter> GetTexture(const wchar_t* texturePath);

	void TextureCleanup();

private:

	ID3D12Device * m_device;
	ID3D12GraphicsCommandList* m_commandList;

	std::map<const wchar_t*, std::shared_ptr<TextureImporter>> textures;

	bool LoadTexture(const wchar_t* texturePath);

};

