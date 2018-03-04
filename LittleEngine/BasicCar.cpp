#include "BasicCar.h"



BasicCar::BasicCar() : LEActor(XMFLOAT4(20.0f, 0.0f, 0.0f, 0.0f))
{
	movementSpeed = 2.0f;
}


BasicCar::~BasicCar()
{
}

void BasicCar::SetPositions(float start, float end)
{
	startPos = start;
	endPos = end;

	transform.Position = XMFLOAT4(start, 0.0f, 0.0f, 0.0f);
}

void BasicCar::Update(float deltaTime)
{
}

void BasicCar::Reset()
{
	transform.Position = XMFLOAT4(startPos, 0.0f, 0.0f, 0.0f);
	transform.Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
}
