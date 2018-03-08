#include "PlayerActor.h"

PlayerActor::PlayerActor() : LEActor(XMFLOAT4(0.0f, 2.0f, -18.0f, 0.0f))
{
	cameraSpeed = 20.0f;
	runSpeedMultiplier = 4.0f;
	cameraPitch = 0.0f;
	pitchSpeed = 0.02f;
	cameraYaw = 0.0f;
	yawSpeed = 0.02f;	

	// set starting camera state
	XMFLOAT4 cameraTarget = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	cameraUp = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);

	// build view matrix
	XMVECTOR cPos = XMLoadFloat4(&transform.Position);
	XMVECTOR cTarg = XMLoadFloat4(&cameraTarget);
	XMVECTOR cUp = XMLoadFloat4(&cameraUp);
	XMStoreFloat4x4(&cameraViewMat, XMMatrixLookAtLH(cPos, cTarg, cUp));

	XMMATRIX viewMat = XMLoadFloat4x4(&cameraViewMat); // load view matrix
													   //Transpose for graphic card
	XMStoreFloat4x4(&m_cbCameraObject->viewMatrix, XMMatrixTranspose(viewMat));

	canMove = false;
}

PlayerActor::~PlayerActor()
{
}

void PlayerActor::Update(float deltaTime)
{
	if (canMove)
	{
		//Get input
		float forward, right, up;
		forward = m_playerInput->Forward(deltaTime) * cameraSpeed;
		right = m_playerInput->Right(deltaTime) * cameraSpeed;
		up = m_playerInput->Up(deltaTime);

		if (m_playerInput->IsShiftKeyDown() || m_playerInput->IsButtonDown(XINPUT_GAMEPAD_Y))
		{
			forward *= runSpeedMultiplier;
			right *= runSpeedMultiplier;
		}

		cameraYaw += m_playerInput->GetRightChangeX(deltaTime) * yawSpeed;
		cameraPitch += m_playerInput->GetRightChangeY(deltaTime) * pitchSpeed;



		//Camera target
		XMMATRIX cameraRotMat = XMMatrixRotationRollPitchYaw(cameraPitch, cameraYaw, 0);
		XMVECTOR camTarget = XMVector3TransformCoord(DefaultForward, cameraRotMat);
		camTarget = XMVector3Normalize(camTarget);

		//Y rotation
		XMMATRIX rotYTempMat;
		rotYTempMat = XMMatrixRotationY(cameraYaw);

		camRight = XMVector3TransformCoord(DefaultRight, rotYTempMat);
		XMVECTOR camUp = XMVector3TransformCoord(XMLoadFloat4(&cameraUp), rotYTempMat);
		camForward = XMVector3TransformCoord(DefaultForward, rotYTempMat);

		XMVECTOR camPos = XMLoadFloat4(&transform.Position);
		camPos += right * camRight;
		camPos += forward * camForward;
		camPos += up * camUp;

		camTarget = camPos + camTarget;

		XMMATRIX tmpMat = XMMatrixLookAtLH(camPos, camTarget, camUp);
		XMStoreFloat4x4(&cameraViewMat, tmpMat);

		XMStoreFloat4(&cameraUp, camUp);
		XMStoreFloat4(&transform.Position, camPos);
	}

	//
	//Camera
	//
	/*
	XMMATRIX viewMat = XMLoadFloat4x4(&cameraViewMat); // load view matrix
	XMMATRIX projMat = XMLoadFloat4x4(&cameraProjMat); // load projection matrix
	//Transpose for graphic card
	XMStoreFloat4x4(&m_cbCameraObject->viewMatrix, XMMatrixTranspose(viewMat));
	XMStoreFloat4x4(&m_cbCameraObject->projMatrix, XMMatrixTranspose(projMat));
	*/
	m_cbCameraObject->cameraPosition = transform.Position;
}

void PlayerActor::Reset()
{
}

void PlayerActor::SetToOthoView()
{
	// build projection and view matrix
	XMMATRIX tmpMat = XMMatrixOrthographicLH(19.2, 10.8, 0.1f, 40.0f);
	XMStoreFloat4x4(&cameraProjMat, tmpMat);
													   //Transpose for graphic card
	XMStoreFloat4x4(&m_cbCameraObject->projMatrix, XMMatrixTranspose(tmpMat));
}

void PlayerActor::SetToPerpsView(float width, float height)
{
	//Change projection matrix to perspective
	XMMATRIX projMat = XMMatrixPerspectiveFovLH(80.0f*(3.14f / 180.0f), width / height, 0.1f, 1000.0f);
	XMStoreFloat4x4(&cameraProjMat, projMat);

	XMStoreFloat4x4(&m_cbCameraObject->projMatrix, XMMatrixTranspose(projMat));
}
