#include "Entity/Component.h"
#include "Entity/Entity.h"

//! @brief �R���X�g���N�^
Component::Component()
{
}

//! @brief �f�X�g���N�^
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
