#pragma once
#include <map>
#include "TextureImporter.h"

namespace Texture
{
	static const wchar_t* LOGO = L"Texture/LittleEngine.png";

	static const wchar_t* FLOOR = L"Texture/city/Tileable7h.png";
	static const wchar_t* WALL = L"Texture/city/Tileable1b.png";
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

