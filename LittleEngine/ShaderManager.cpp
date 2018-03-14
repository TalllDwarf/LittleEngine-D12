#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
}


ShaderManager::~ShaderManager()
{
}

bool ShaderManager::Initialise(ID3D12Device * device, DXGI_SAMPLE_DESC sampleDesc)
{
	
	// -- Create root signiture -- //
	HRESULT hr;

	//Create a root descriptor, which explains where to find the data for this root parameter
	D3D12_ROOT_DESCRIPTOR cameraCBVDescriptor;
	cameraCBVDescriptor.RegisterSpace = 0;
	cameraCBVDescriptor.ShaderRegister = 0;

	D3D12_ROOT_DESCRIPTOR lightCBVDescriptor;
	lightCBVDescriptor.RegisterSpace = 0;
	lightCBVDescriptor.ShaderRegister = 1;

	D3D12_ROOT_DESCRIPTOR objectCBVDescriptor;
	objectCBVDescriptor.RegisterSpace = 0;
	objectCBVDescriptor.ShaderRegister = 2;	

	D3D12_DESCRIPTOR_RANGE descriptorTableRange[1]; //only one range
	descriptorTableRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV; //this is a range of shader resource view
	descriptorTableRange[0].NumDescriptors = 2; // We only have one texture right now so the range is only 1
	descriptorTableRange[0].BaseShaderRegister = 0; //Start index of the shader register in range
	descriptorTableRange[0].RegisterSpace = 0; //Space 0 can usually be zero
	descriptorTableRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND; //This appends the range to the end of the root signature descriptor table
	

	D3D12_ROOT_DESCRIPTOR_TABLE descriptorTable;
	descriptorTable.NumDescriptorRanges = _countof(descriptorTableRange);
	descriptorTable.pDescriptorRanges = &descriptorTableRange[0];

	//create a root parameter and fill it out
	D3D12_ROOT_PARAMETER rootParameters[4]; // Only one parameter
	//Camera
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // This is a constant buffer view root descriptor
	rootParameters[0].Descriptor = cameraCBVDescriptor; // This is the root descriptor for this root parameter
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	
	//Light
	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // This is a constant buffer view root descriptor
	rootParameters[1].Descriptor = lightCBVDescriptor; // This is the root descriptor for this root parameter
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	
	//PerObject
	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV; // This is a constant buffer view root descriptor
	rootParameters[2].Descriptor = objectCBVDescriptor; // This is the root descriptor for this root parameter
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	
	//Textures
	rootParameters[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[3].DescriptorTable = descriptorTable;
	rootParameters[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	D3D12_STATIC_SAMPLER_DESC sampler = {};
	sampler.Filter = D3D12_FILTER_ANISOTROPIC;  //D3D12_FILTER_MINIMUM_MIN_MAG_MIP_LINEAR;
	sampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	sampler.MipLODBias = 0;
	sampler.MaxAnisotropy = 0;
	sampler.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	sampler.BorderColor =  D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
	sampler.MinLOD = 0.0f;
	sampler.MaxLOD = D3D12_FLOAT32_MAX;
	sampler.ShaderRegister = 0;
	sampler.RegisterSpace = 0;
	sampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init(
		_countof(rootParameters),
		rootParameters,
		1,
		&sampler,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | // we can deny shader stages here for better performance
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS);

	ID3DBlob* errorBuff; // a buffer holding the error data if any
	ID3DBlob* signature;
	hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &errorBuff);
	if (FAILED(hr))
	{
		OutputDebugStringA((char*)errorBuff->GetBufferPointer());
		return false;
	}

	hr = device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	if (FAILED(hr))
	{
		return false;
	}

	//
	//Texture Shader
	//
	
	{
		std::shared_ptr<TextureShader> texShader = std::make_shared<TextureShader>();

		if (!texShader->Initialise(device, rootSignature, sampleDesc))
		{
			return false;
		}

		shaders[ShaderTypes::Texture_Shader] = texShader;
	}
	

	//
	//Light Texture
	//
	{
		std::shared_ptr<LightShader> lightShader = std::make_shared<LightShader>();

		if (!lightShader->Initialise(device, rootSignature, sampleDesc))
		{
			return false;
		}

		shaders[ShaderTypes::Light_Texture_Shader] = lightShader;
	}

	//
	//Bump Texture
	//
	{
		std::shared_ptr<BumpShader> bumpShader = std::make_shared<BumpShader>();

		if (!bumpShader->Initialise(device, rootSignature, sampleDesc))
		{
			return false;
		}

		shaders[ShaderTypes::Bump_Texture_Shader] = bumpShader;
	}

	return true;
}

ID3D12PipelineState * ShaderManager::GetPipelineState(ShaderTypes type)
{
	return shaders[type]->GetPipelineState();
}

void ShaderManager::CleanUp()
{
	SAFE_RELEASE(rootSignature);
}