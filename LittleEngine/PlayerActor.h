#pragma once
#include "LEActor.h"
#include "Input.h"

class PlayerActor : public LEActor
{
	//Input
	Input * m_playerInput;

	XMFLOAT4X4 cameraProjMat; // this will store our projection matrix
	XMFLOAT4X4 cameraViewMat; // this will store our view matrix

	XMFLOAT4 cameraUp; // the worlds up vector

	XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	XMVECTOR DefaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	XMVECTOR camRight;
	XMVECTOR camForward;

	float cameraSpeed;
	float runSpeedMultiplier;
	float cameraPitch, cameraYaw;
	float pitchSpeed, yawSpeed;

	ConstantCameraBuffer *m_cbCameraObject;

	bool isOnFloor;
	bool jumping;
	bool falling;
	bool canMove;

public:
	PlayerActor();
	~PlayerActor();

	virtual void Update(float deltaTime) override;

	virtual void Reset() override;

	//Change camera views
	void SetToOthoView();
	void SetToPerpsView(float width, float height);

	//Can the camera move
	void CanPlayerMove(bool moveable) { canMove = moveable; }

	//Get the input controller
	void GetInput(Input* playerinput) { m_playerInput = playerinput; }

	//Get the camera buffer giving us better controle
	void SetBuffer(ConstantCameraBuffer* cbCameraBuffer);

};

