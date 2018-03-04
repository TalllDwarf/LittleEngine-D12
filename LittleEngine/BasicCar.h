#pragma once
#include "LEActor.h"
class BasicCar :
	public LEActor
{
public:
	BasicCar();
	~BasicCar();
	
	void SetPositions(float start, float end);

	virtual void Update(float deltaTime) override;

	virtual void Reset() override;

private:

	float movementSpeed;

	float startPos;
	float endPos;


};

