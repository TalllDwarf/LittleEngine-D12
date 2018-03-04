#pragma once
#include "ModelImporter.h"

class CollisionAABB
{
public:
	CollisionAABB();
	~CollisionAABB();

	//XMFLOAT3 minLocalBoundingBox;
	//XMFLOAT3 maxLocalBoundingBox;
	std::vector<XMFLOAT3> localBoundingBox;

	XMFLOAT3 minWorldBoundingBox;
	XMFLOAT3 maxWorldBoundingBox;

	void CreateAABB(std::shared_ptr<ModelImporter> model);

	void CalculateWorldAABB(XMMATRIX worldSpace);

};

