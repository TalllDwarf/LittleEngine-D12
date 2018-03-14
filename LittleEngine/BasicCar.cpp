#include "BasicCar.h"



BasicCar::BasicCar() : LEActor(XMFLOAT4(20.0f, 0.0f, 0.0f, 0.0f))
{

}


BasicCar::~BasicCar()
{
}

void BasicCar::Update(float deltaTime)
{
	transform.Rotation.y += (0.001 * deltaTime);
}

void BasicCar::Reset()
{
	transform.Rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
}
