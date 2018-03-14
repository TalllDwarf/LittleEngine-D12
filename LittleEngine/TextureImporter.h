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
	bool LoadTexture(const wchar_t* texturePath, const wchar_t* bumpmapPath = nullptr);

	bool LoadBumpmap(const wchar_t* bumpmapPath, CD3DX12_CPU_DESCRIPTOR_HANDLE& hDescriptor);

	bool hasBumpmap() { return hasBump; }

	//Sets the texture in the curent descriptor heap
	void SetDescriptorHeap(UINT rootParameterIndex);

	//Clean up textures
	void TextureCleanup();

private:

	ID3D12Device * m_device;
	ID3D12GraphicsCommandList* m_commandList;

	UINT descriptorSize;

	bool hasBump;

	//Image data
	BYTE * imageData;
	BYTE * bumpmapData;

	//Descriptor heap - contains texture
	ID3D12DescriptorHeap* m_textureDescriptorHeap;

	ID3D12Resource* m_textureBuffer;
	ID3D12Resource* m_textureBufferUploadHeap;

	ID3D12Resource* m_bumpmapBuffer;
	ID3D12Resource* m_bumpmapBufferUploadHeap;
};

