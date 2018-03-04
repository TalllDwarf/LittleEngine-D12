#pragma once
#include <map>
#include "stdafx.h"
#include "ModelImporter.h"

namespace Model
{
	static const char* PLANE = "Model/Plane.obj";
	static const char* CUBE = "Model/Cube.obj";
	static const char* CAR1 = "Model/Car_1.obj";
	static const char* CAR2 = "Model/Car_2.obj";
	static const char* CAR3 = "Model/Car_3.obj";
	static const char* CAR4 = "Model/Car_4.obj";
};

class ModelManager
{

public:
	ModelManager();
	~ModelManager();

	//Set device and commandlist variables
	//Then loads all available models
	bool Initialise(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);

	//Return a loaded model
	std::shared_ptr<ModelImporter> GetModel(const char* modelPath);	

	void CleanUp();

private:

	bool LoadModel(const char* model);

	ID3D12Device * m_device;
	ID3D12GraphicsCommandList* m_commandList;

	//All of our loaded models
	std::map<const char*, std::shared_ptr<ModelImporter>> models;	

};

