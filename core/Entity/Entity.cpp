#include "Entity/Entity.h"
#include "Entity/Component.h"
#include "Game/Game.h"
#include "Game/Scene.h"
#include "Entity/TransformComponent.h"

//! @brief コンストラクタ
Entity::Entity()
{
	// Transcomponentはデフォルトで追加する
	m_Transform = AddComponent<TransformComponent>();
}

//! @brief デストラクタ
Entity::~Entity()
{
}

void Entity::Release()
{
	m_pScene->RemoveEntity(this);
}

InputSystem* Entity::GetInputSystem()
{
	return m_pScene->GetGame()->GetInputSystem();
}

void Entity::AddComponentInternal(Component* component, size_t id)
{
	auto compPtr = std::unique_ptr<Component>(component);
	m_Components.emplace(id, std::move(compPtr));
	component->m_TypeId = id;
	component->m_pEnity = this;

	component->onCreateInternal();
}

Component* Entity::GetComponentInternal(size_t id)
{
	auto it = m_Components.find(id);
	if (it == m_Components.end())
		return nullptr;

	return it->second.get();
}

void Entity::RemoveComponent(size_t id)
{
	m_Components.erase(id);
}

