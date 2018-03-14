#pragma once
#include "LEActor.h"
class BasicCar :
	public LEActor
{
public:
	BasicCar();
	~BasicCar();

	virtual void Update(float deltaTime) override;

	virtual void Reset() override;

private:

	float movementSpeed;

	float startPos;
	float endPos;


};

