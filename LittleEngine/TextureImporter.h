#pragma once
#include "stdafx.h"

class TextureImporter
{
public:
	TextureImporter();
	~TextureImporter();

	//Gets the device and command list then loads all of the models
	void Initialise(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);

	//Loads the texture file
	bool LoadTexture(const wchar_t* texturePath);

	//Sets the texture in the curent descriptor heap
	void SetDescriptorHeap(UINT rootParameterIndex);

	//Clean up textures
	void TextureCleanup();

private:

	ID3D12Device * m_device;
	ID3D12GraphicsCommandList* m_commandList;

	//Image data
	BYTE * imageData;

	//Descriptor heap - contains texture
	ID3D12DescriptorHeap* m_mainDescriptorHeap;

	ID3D12Resource* m_textureBuffer;
	ID3D12Resource* m_textureBufferUploadHeap;
};

