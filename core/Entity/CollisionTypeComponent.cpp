#include "Entity/CollisionTypeComponent.h"
#include "Entity/Entity.h"
#include "Game/Game.h"
#include "Graphics/GraphicsEngine.h"
#include "System/PhysicsEngine.h"
#include "Game/Scene.h"

CollisionTypeComponent::CollisionTypeComponent()
{
}

CollisionTypeComponent::~CollisionTypeComponent()
{
	m_pEnity->GetScene()->GetGame()->GetPhysicsEngine()->RemoveComponent(this);
}

void CollisionTypeComponent::onCreateInternal()
{
	m_pEnity->GetScene()->GetGame()->GetPhysicsEngine()->AddComponent(this);
}
