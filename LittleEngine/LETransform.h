#pragma once
#include "stdafx.h"

class LETransform
{
public:
	LETransform(XMFLOAT4 position, XMFLOAT3 rotation, XMFLOAT3 scale);
	~LETransform();

	XMFLOAT4 Position;
	XMFLOAT3 Rotation;
	XMFLOAT3 Scale;

	void MoveActorLocation(XMFLOAT4 addMovement);

	void MoveActorLocation(float x, float y, float z);

	//Creates and returns the world matrix for the Transform
	XMMATRIX GetWorldMatrix();

private:

	
};

