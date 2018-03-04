#pragma once
#include "LEActor.h"

class LALogo : public LEActor
{
public:
	LALogo();
	~LALogo();

	virtual void Update(float deltaTime) override;

	virtual void Reset() override;
	
	bool HasFinished() { return finished; }

private:

	float delayTimer;
	float endX;

	bool finished;
};

