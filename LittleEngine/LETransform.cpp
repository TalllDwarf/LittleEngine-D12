#include "LETransform.h"

LETransform::LETransform(XMFLOAT4 position, XMFLOAT3 rotation, XMFLOAT3 scale)
{
	Position =  position;
	Rotation =  rotation;
	Scale =  scale;
}

LETransform::~LETransform()
{
}

void LETransform::MoveActorLocation(XMFLOAT4 addMovement)
{
	Position.x += addMovement.x;
	Position.y += addMovement.y;
	Position.z += addMovement.z;
}

void LETransform::MoveActorLocation(float x, float y, float z)
{
	Position.x += x;
	Position.y += y;
	Position.z += z;
}
//Creates and returns the world matrix for the transform
XMMATRIX LETransform::GetWorldMatrix()
{
	XMMATRIX worldMatrix = XMMatrixScaling(Scale.x, Scale.y, Scale.z);
	worldMatrix = (worldMatrix * XMMatrixRotationRollPitchYaw(Rotation.y, Rotation.z, Rotation.x));
	worldMatrix = (worldMatrix * XMMatrixTranslationFromVector(XMLoadFloat4(&Position)));

	return worldMatrix;
}