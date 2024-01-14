#include "Entity/TransformComponent.h"
#include "Entity/Entity.h"
#include "Game/Game.h"
#include "Graphics/GraphicsEngine.h"
#include "System/PhysicsEngine.h"
#include "Game/Scene.h"

TransformComponent::TransformComponent()
{
}

TransformComponent::~TransformComponent()
{
}

void TransformComponent::ComputeWorldTransform()
{
	Matrix temp = Matrix::Identity;

	m_WorldTransform = Matrix::Identity;
	// まずはスケーリング，次に回転，最後に平行移動
	temp = DirectX::XMMatrixScaling(m_Scale.x, m_Scale.y, m_Scale.z);
	m_WorldTransform *= temp;

	temp = Matrix::Identity;
	temp = DirectX::XMMatrixRotationQuaternion(m_Rotation);
	m_WorldTransform *= temp;

	temp = Matrix::Identity;
	temp = DirectX::XMMatrixTranslation(m_Position.x, m_Position.y, m_Position.z);
	m_WorldTransform *= temp;
}
