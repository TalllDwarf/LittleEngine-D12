#pragma once
#include <map>
#include "TextureImporter.h"

namespace Texture
{
	static const wchar_t* LOGO = L"Texture/LittleEngine.png";

	static const wchar_t* FLOOR = L"Texture/T.gif";//  L"Texture/city/Tileable7a.png";
	static const wchar_t* WALL = L"Texture/T.gif";// L"Texture/city/Tileable1b.png";
}

namespace Bumpmap
{
	static const wchar_t* WALL = L"Texture/N.gif"; // L"Texture/normals/Tileable1_nm.png";
	static const wchar_t* FLOOR = L"Texture/N.gif";  //L"Texture/normals/Tileable7_nm.png";
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

	bool LoadTexture(const wchar_t* texturePath, const wchar_t* bumpPath = nullptr);

};

