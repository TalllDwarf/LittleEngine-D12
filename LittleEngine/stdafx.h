#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN  // Exclude rarely-used stuff from Windows headers.
#endif

#include <Windows.h>
#include <memory>
#include <dxgi1_4.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include "d3dx12.h"
#include <wincodec.h>
#include <string>
#include <fstream>

#define SAFE_RELEASE(p) { if ((p)) { p->Release(); p = NULL; }}

using namespace DirectX;

//Simpler operations
inline XMFLOAT3 Sub(XMFLOAT3 a, XMFLOAT3 b)
{
	return XMFLOAT3(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline XMFLOAT3 Sum(XMFLOAT3 a, XMFLOAT3 b)
{
	return XMFLOAT3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline XMFLOAT3 Multi(XMFLOAT3 a, float b)
{
	return XMFLOAT3(a.x * b, a.y * b, a.z * b);
}

struct Vertex {
	Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz, float tx, float ty, float tz) 
		: pos(x, y, z), texCoord(u, v), normal(nx, ny, nz), tangent(tx,ty,tz) {}
	XMFLOAT3 pos;
	XMFLOAT2 texCoord;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
	XMFLOAT3 binormal;
};

struct TempVertex {
	XMFLOAT3 pos;
	XMFLOAT2 texCoord;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
	XMFLOAT3 binormal;
};

inline TempVertex ToTemp(Vertex a)
{
	TempVertex temp;
	temp.pos = a.pos;
	temp.texCoord = a.texCoord;
	temp.normal = a.normal;
	temp.tangent = a.tangent;
	temp.binormal = a.binormal;

	return temp;
}

// this is the structure of our constant buffer.
struct ConstantCameraBuffer
{
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projMatrix;
	XMFLOAT4 cameraPosition;
};

struct ConstantLightBuffer
{
	XMFLOAT4 ambientColor;
	XMFLOAT4 diffuseColor;
	XMFLOAT3 lightDirection;
	float specularPower;
	XMFLOAT4 specularColor;
	XMFLOAT3 lightPosition;
	float range;
	XMFLOAT3 att;
};

struct ConstantSpotLightBuffer
{
	XMFLOAT3 pos;
	float range;
	XMFLOAT3 dir;
	float cone;
	XMFLOAT3 att;
	float pad2;
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
};

struct ConstantBufferObject 
{
	XMFLOAT4X4 worldMatrix;
};

static DXGI_FORMAT GetDXGIFormatFromWICFormat(WICPixelFormatGUID& wicFormatGUID)
{
	if (wicFormatGUID == GUID_WICPixelFormat128bppRGBAFloat)
		return DXGI_FORMAT_R32G32B32A32_FLOAT;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppRGBAHalf)
		return DXGI_FORMAT_R16G16B16A16_FLOAT;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppRGBA)
		return DXGI_FORMAT_R16G16B16A16_UNORM;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppRGBA)
		return DXGI_FORMAT_R8G8B8A8_UNORM;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppBGRA)
		return DXGI_FORMAT_B8G8R8A8_UNORM;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppBGR)
		return DXGI_FORMAT_B8G8R8X8_UNORM;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppRGBA1010102XR)
		return DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppRGBA1010102)
		return DXGI_FORMAT_R10G10B10A2_UNORM;
	else if (wicFormatGUID == GUID_WICPixelFormat16bppBGRA5551)
		return DXGI_FORMAT_B5G5R5A1_UNORM;
	else if (wicFormatGUID == GUID_WICPixelFormat16bppBGR565)
		return DXGI_FORMAT_B5G6R5_UNORM;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppGrayFloat)
		return DXGI_FORMAT_R32_FLOAT;
	else if (wicFormatGUID == GUID_WICPixelFormat16bppGrayHalf)
		return DXGI_FORMAT_R16_FLOAT;
	else if (wicFormatGUID == GUID_WICPixelFormat16bppGray)
		return DXGI_FORMAT_R16_UNORM;
	else if (wicFormatGUID == GUID_WICPixelFormat8bppGray)
		return DXGI_FORMAT_R8_UNORM;
	else if (wicFormatGUID == GUID_WICPixelFormat8bppAlpha)
		return DXGI_FORMAT_A8_UNORM;
	else
		return DXGI_FORMAT_UNKNOWN;
}

static WICPixelFormatGUID GetConvertToWICFormat(WICPixelFormatGUID& wicFormatGUID)
{
	if (wicFormatGUID == GUID_WICPixelFormatBlackWhite)
		return GUID_WICPixelFormat8bppGray;
	else if (wicFormatGUID == GUID_WICPixelFormat1bppIndexed)
		return GUID_WICPixelFormat32bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat2bppIndexed)
		return GUID_WICPixelFormat32bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat4bppIndexed)
		return GUID_WICPixelFormat32bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat8bppIndexed)
		return GUID_WICPixelFormat32bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat2bppGray)
		return GUID_WICPixelFormat8bppGray;
	else if (wicFormatGUID == GUID_WICPixelFormat4bppGray)
		return GUID_WICPixelFormat8bppGray;
	else if (wicFormatGUID == GUID_WICPixelFormat16bppGrayFixedPoint)
		return GUID_WICPixelFormat16bppGrayHalf;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppGrayFixedPoint)
		return GUID_WICPixelFormat32bppGrayFloat;
	else if (wicFormatGUID == GUID_WICPixelFormat16bppBGR555)
		return GUID_WICPixelFormat16bppBGRA5551;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppBGR101010)
		return GUID_WICPixelFormat32bppRGBA1010102;
	else if (wicFormatGUID == GUID_WICPixelFormat24bppBGR)
		return GUID_WICPixelFormat32bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat24bppRGB)
		return GUID_WICPixelFormat32bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppPBGRA)
		return GUID_WICPixelFormat32bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppPRGBA)
		return GUID_WICPixelFormat32bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat48bppRGB)
		return GUID_WICPixelFormat64bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat48bppBGR)
		return GUID_WICPixelFormat64bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppBGRA)
		return GUID_WICPixelFormat64bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppPRGBA)
		return GUID_WICPixelFormat64bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppPBGRA)
		return GUID_WICPixelFormat64bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat48bppRGBFixedPoint)
		return GUID_WICPixelFormat64bppRGBAHalf;
	else if (wicFormatGUID == GUID_WICPixelFormat48bppBGRFixedPoint)
		return GUID_WICPixelFormat64bppRGBAHalf;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppRGBAFixedPoint)
		return GUID_WICPixelFormat64bppRGBAHalf;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppBGRAFixedPoint)
		return GUID_WICPixelFormat64bppRGBAHalf;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppRGBFixedPoint)
		return GUID_WICPixelFormat64bppRGBAHalf;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppRGBHalf)
		return GUID_WICPixelFormat64bppRGBAHalf;
	else if (wicFormatGUID == GUID_WICPixelFormat48bppRGBHalf)
		return GUID_WICPixelFormat64bppRGBAHalf;
	else if (wicFormatGUID == GUID_WICPixelFormat128bppPRGBAFloat)
		return GUID_WICPixelFormat128bppRGBAFloat;
	else if (wicFormatGUID == GUID_WICPixelFormat128bppRGBFloat)
		return GUID_WICPixelFormat128bppRGBAFloat;
	else if (wicFormatGUID == GUID_WICPixelFormat128bppRGBAFixedPoint)
		return GUID_WICPixelFormat128bppRGBAFloat;
	else if (wicFormatGUID == GUID_WICPixelFormat128bppRGBFixedPoint)
		return GUID_WICPixelFormat128bppRGBAFloat;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppRGBE)
		return GUID_WICPixelFormat128bppRGBAFloat;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppCMYK)
		return GUID_WICPixelFormat32bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppCMYK)
		return GUID_WICPixelFormat64bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat40bppCMYKAlpha)
		return GUID_WICPixelFormat64bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat80bppCMYKAlpha)
		return GUID_WICPixelFormat64bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat32bppRGB)
		return GUID_WICPixelFormat32bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppRGB)
		return GUID_WICPixelFormat64bppRGBA;
	else if (wicFormatGUID == GUID_WICPixelFormat64bppPRGBAHalf)
		return GUID_WICPixelFormat64bppRGBAHalf;
	else
		return GUID_WICPixelFormatDontCare;
}

static int GetDXGIFormatBitsPerPixel(DXGI_FORMAT& dxgiFormat)
{
	if (dxgiFormat == DXGI_FORMAT_R32G32B32A32_FLOAT)
		return 128;
	else if (dxgiFormat == DXGI_FORMAT_R16G16B16A16_FLOAT)
		return 64;
	else if (dxgiFormat == DXGI_FORMAT_R16G16B16A16_UNORM)
		return 64;
	else if (dxgiFormat == DXGI_FORMAT_R8G8B8A8_UNORM)
		return 32;
	else if (dxgiFormat == DXGI_FORMAT_B8G8R8A8_UNORM)
		return 32;
	else if (dxgiFormat == DXGI_FORMAT_B8G8R8X8_UNORM)
		return 32;
	else if (dxgiFormat == DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM)
		return 32;
	else if (dxgiFormat == DXGI_FORMAT_R10G10B10A2_UNORM)
		return 32;
	else if (dxgiFormat == DXGI_FORMAT_B5G5R5A1_UNORM)
		return 16;
	else if (dxgiFormat == DXGI_FORMAT_B5G6R5_UNORM)
		return 16;
	else if (dxgiFormat == DXGI_FORMAT_R32_FLOAT)
		return 32;
	else if (dxgiFormat == DXGI_FORMAT_R16_FLOAT)
		return 16;
	else if (dxgiFormat == DXGI_FORMAT_R16_UNORM)
		return 16;
	else if (dxgiFormat == DXGI_FORMAT_R8_UNORM)
		return 8;
	else if (dxgiFormat == DXGI_FORMAT_A8_UNORM)
		return 8;
}

static int LoadImageDataFromFile(BYTE** pImageData, D3D12_RESOURCE_DESC& resourceDescription, LPCWSTR filename, int& bytesPerRow)
{
	HRESULT hr;

	static IWICImagingFactory *pWICFactory;

	IWICBitmapDecoder* pWICDecoder = NULL;
	IWICBitmapFrameDecode* pWICFrame = NULL;
	IWICFormatConverter* pWICConverter = NULL;

	bool imageConverted = false;

	if (pWICFactory == NULL)
	{
		// Initialize the COM library
		CoInitialize(NULL);

		// Create the WIC Factory
		hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pWICFactory));
		if (FAILED(hr))
		{
			return 0;
		}
	}

	// Load a decoder for the image
	hr = pWICFactory->CreateDecoderFromFilename(filename, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pWICDecoder);
	if (FAILED(hr))
	{
		return 0;
	}

	// Get the image from the decoder
	hr = pWICDecoder->GetFrame(0, &pWICFrame);
	if (FAILED(hr))
	{
		return 0;
	}

	// Get the WIC Pixel Format of the image
	WICPixelFormatGUID pixelFormat;
	hr = pWICFrame->GetPixelFormat(&pixelFormat);
	if (FAILED(hr))
	{
		return 0;
	}

	// Get size of image
	UINT textureWidth, textureHeight;
	hr = pWICFrame->GetSize(&textureWidth, &textureHeight);
	if (FAILED(hr))
	{
		return 0;
	}

	// Get DXGI format using WIC Format
	DXGI_FORMAT dxgiFormat = GetDXGIFormatFromWICFormat(pixelFormat);

	// Check for unknown format
	if (dxgiFormat == DXGI_FORMAT_UNKNOWN)
	{
		WICPixelFormatGUID convertToPixelFormat = GetConvertToWICFormat(pixelFormat);

		if (convertToPixelFormat == GUID_WICPixelFormatDontCare)
			return 0;

		// Set the format
		dxgiFormat = GetDXGIFormatFromWICFormat(convertToPixelFormat);

		// Create the converter
		hr = pWICFactory->CreateFormatConverter(&pWICConverter);
		if (FAILED(hr))
		{
			return 0;
		}

		BOOL canConvert = FALSE;
		hr = pWICConverter->CanConvert(pixelFormat, convertToPixelFormat, &canConvert);
		if (FAILED(hr) || !canConvert)
		{
			return 0;
		}

		// Convert it
		hr = pWICConverter->Initialize(pWICFrame, convertToPixelFormat, WICBitmapDitherTypeErrorDiffusion, 0, 0, WICBitmapPaletteTypeCustom);
		if (FAILED(hr))
		{
			return 0;
		}

		imageConverted = true;
	}

	int bitsPerPixel = GetDXGIFormatBitsPerPixel(dxgiFormat);
	bytesPerRow = (textureWidth * bitsPerPixel) / 8;
	int imageSize = bytesPerRow * textureHeight;

	// Allocate memory for image
	*pImageData = (BYTE*)malloc(imageSize);

	if (imageConverted)
	{
		hr = pWICConverter->CopyPixels(0, bytesPerRow, imageSize, *pImageData);
		if (FAILED(hr))
		{
			return 0;
		}
	}
	else
	{
		hr = pWICFrame->CopyPixels(0, bytesPerRow, imageSize, *pImageData);
		if (FAILED(hr))
		{
			return 0;
		}
	}

	resourceDescription = {};
	resourceDescription.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDescription.Alignment = 0;
	resourceDescription.Width = textureWidth;
	resourceDescription.Height = textureHeight;
	resourceDescription.DepthOrArraySize = 1;
	resourceDescription.MipLevels = 1;
	resourceDescription.Format = dxgiFormat;
	resourceDescription.SampleDesc.Count = 1;
	resourceDescription.SampleDesc.Quality = 0;
	resourceDescription.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resourceDescription.Flags = D3D12_RESOURCE_FLAG_NONE;

	return imageSize;
}