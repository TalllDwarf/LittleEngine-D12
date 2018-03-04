#include "LEActor.h"

LEActor::LEActor() : transform(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)), Velocity(XMFLOAT3(0.0f,0.0f,0.0f))
{}

LEActor::LEActor(XMFLOAT4 startPosition) : transform(startPosition, XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT3(1.0f, 1.0f, 1.0f)), Velocity(XMFLOAT3(0.0f, 0.0f, 0.0f))
{}

LEActor::LEActor(XMFLOAT4 startPosition, XMFLOAT3 startRotation) : transform(startPosition, startRotation, XMFLOAT3(1.0f,1.0f,1.0f)), Velocity(XMFLOAT3(0.0f, 0.0f, 0.0f))
{}

LEActor::LEActor(XMFLOAT4 startPosition, XMFLOAT3 startRotation, XMFLOAT3 startScale) :transform(startPosition,startRotation,startScale), Velocity(XMFLOAT3(0.0f, 0.0f, 0.0f))
{}

LEActor::~LEActor()
{}

void LEActor::Initialise(ID3D12GraphicsCommandList* commandList, std::shared_ptr<ModelImporter> actorModel, std::shared_ptr<TextureImporter> modelTexture = nullptr)
{
	m_commandList = commandList;
	model = actorModel;
	texture = modelTexture;
}

void LEActor::InitialiseAABBCollision()
{
	collisionAABB = std::make_shared<CollisionAABB>();
	collisionAABB->CreateAABB(model);
}

void LEActor::InitialiseSphereCollision()
{
	collisionSphere = std::make_shared<CollisionSphere>();
	collisionSphere->CreateSphereCollider(model);
}

void LEActor::UpdateCollision(XMMATRIX& worldMatrix)
{
	if (collisionAABB != nullptr)
	{
		collisionAABB->CalculateWorldAABB(worldMatrix);
	}

	if (collisionSphere != nullptr)
	{
		//TODO: call sphere update
	}
}

void LEActor::Render(UINT textureRootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS GPUVirtualAdress)
{
	if (texture != nullptr) 
	{
		texture->SetDescriptorHeap(textureRootParameterIndex);
	}

	model->SetBuffers();

	// set the actors's constant buffer
	m_commandList->SetGraphicsRootConstantBufferView(2, GPUVirtualAdress + ConstantBufferOffset);

	// draw actor
	m_commandList->DrawIndexedInstanced(model->GetIndeciesCount(), 1, 0, 0, 0);
}