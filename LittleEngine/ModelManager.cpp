#include "ModelManager.h"



ModelManager::ModelManager()
{
}


ModelManager::~ModelManager()
{
}

bool ModelManager::Initialise(ID3D12Device * device, ID3D12GraphicsCommandList * commandList)
{
	m_device = device;
	m_commandList = commandList;

	if (!LoadModel(Model::PLANE))
	{
		return false;
	}

	if (!LoadModel(Model::CUBE))
	{
		return false;
	}

	if (!LoadModel(Model::CAR1))
	{
		return false;
	}

	if (!LoadModel(Model::CAR2))
	{
		return false;
	}

	if (!LoadModel(Model::CAR3))
	{
		return false;
	}

	if (!LoadModel(Model::CAR4))
	{
		return false;
	}

	return true;
}

//Get a model that uses a specific path
std::shared_ptr<ModelImporter> ModelManager::GetModel(const char* modelPath)
{
	return models[modelPath];
}

//Tells each model importer to clean up
void ModelManager::CleanUp()
{
	for (std::map<const char*, std::shared_ptr<ModelImporter>>::iterator it = models.begin(); it != models.end(); it++)
	{
		it->second->CleanUp();
	}
}

//Create a model loader and tell it what model we want it to load
bool ModelManager::LoadModel(const char* model)
{
	std::shared_ptr<ModelImporter> temp = std::make_shared<ModelImporter>();
	temp->Init(m_device, m_commandList);

	if (!temp->OpenFile(model))
	{
		return false;
	}

	models[model] = temp;
	return true;
}
