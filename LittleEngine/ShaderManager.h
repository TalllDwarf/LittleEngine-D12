#pragma once
#include <map>
#include "stdafx.h"
#include "TextureShader.h"
#include "LightShader.h"
#include "BumpShader.h"

class ShaderManager
{

public:
	ShaderManager();
	~ShaderManager();

	enum ShaderTypes
	{
		Texture_Shader,
		Light_Texture_Shader,
		Bump_Texture_Shader
	};

	bool Initialise(ID3D12Device* device, DXGI_SAMPLE_DESC sampleDesc);

	ID3D12PipelineState* GetPipelineState(ShaderTypes type);

	ID3D12RootSignature* GetRootSignature() { return rootSignature; }

	void CleanUp();


private:

	ID3D12RootSignature * rootSignature; // root signature defines data shaders will access

	std::map<int, std::shared_ptr<Shader>> shaders;

};

