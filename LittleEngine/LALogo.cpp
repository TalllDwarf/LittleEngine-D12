	#include "LALogo.h"


LALogo::LALogo() : LEActor(XMFLOAT4(17.0f,0.0f,0.0f,0.0f))
{
	Velocity.x = 10.0f;
	delayTimer =2.5f;
	endX = -17;
	finished = false;
}

LALogo::~LALogo()
{
}

void LALogo::Update(float deltaTime)
{
	if (transform.Position.x > 0)
	{
		transform.Position.x -= (Velocity.x * deltaTime);
	}
	else if(delayTimer > 0)
	{
		delayTimer -= deltaTime;
	}
	else
	{
		transform.Position.x -= (Velocity.x * deltaTime);
		if (transform.Position.x < endX)
		{
			finished = true;
		}
	}
}

void LALogo::Reset()
{
	transform.Position.x = 17.0f;
	delayTimer = 2.5f;
	finished = false;
}
