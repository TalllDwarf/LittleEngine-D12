#pragma once

#include "ModelImporter.h"
#include "TextureImporter.h"
#include "LETransform.h"
#include "CollisionAABB.h"
#include "CollisionSphere.h"

class LEActor
{
public:
	LEActor();
	LEActor(XMFLOAT4 startPosition);
	LEActor(XMFLOAT4 startPosition, XMFLOAT3 startRotation);
	LEActor(XMFLOAT4 startPosition, XMFLOAT3 startRotation, XMFLOAT3 startScale);
	~LEActor();

	LETransform transform;
	XMFLOAT3 Velocity;

	void Initialise(ID3D12GraphicsCommandList* commandList, std::shared_ptr<ModelImporter> actorModel, std::shared_ptr<TextureImporter> modelTexture);

	//Creates AABB collision for the actor
	virtual void InitialiseAABBCollision();

	//Creates Sphere collision for the actor
	virtual void InitialiseSphereCollision();

	//Updates our collision with the new world matrix
	virtual void UpdateCollision(XMMATRIX& worldMatrix);

	//sets the constant buffer position where this actors data is stored
	void SetConstantBufferOffset(UINT constantBufferOffset) { ConstantBufferOffset = constantBufferOffset; };
	UINT GetConstantBufferOffset() { return ConstantBufferOffset; }

	//Update function needs to be implemented by all actors
	virtual void Update(float deltaTime) {};

	//Render the model to the screen
	void Render(UINT textureRootParameterIndex, D3D12_GPU_VIRTUAL_ADDRESS GPUVirtualAdress);

	//Resets the Actor to its starting position
	virtual void Reset() {};

	//Return our colliders
	std::shared_ptr<CollisionAABB> GetAABBCollider() { return collisionAABB; }
	std::shared_ptr<CollisionSphere> GetSphereCollider() { return collisionSphere; }

	//Does our actor have a collider
	bool hasAABB() { return (collisionAABB != nullptr); }
	bool hasSphereCollision() { return (collisionSphere != nullptr); }

private:

	UINT ConstantBufferOffset;

	ID3D12GraphicsCommandList* m_commandList;

	std::shared_ptr<ModelImporter> model;
	std::shared_ptr<TextureImporter> texture;

	std::shared_ptr<CollisionAABB> collisionAABB;
	std::shared_ptr<CollisionSphere> collisionSphere;
};

