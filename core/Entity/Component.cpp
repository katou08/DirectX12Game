#include "Entity/Component.h"
#include "Entity/Entity.h"

//! @brief コンストラクタ
Component::Component()
{
}

//! @brief デストラクタ
Component::~Component()
{
}

void Component::Release()
{
	m_pEnity->RemoveComponent(m_TypeId);
}

void Component::onCreateInternal()
{
}
