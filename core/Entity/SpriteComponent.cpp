#include "Entity/SpriteComponent.h"
#include "Resource/Sprite.h"
#include "Entity/Entity.h"
#include "Game/Game.h"
#include "Graphics/GraphicsEngine.h"
#include "Game/Scene.h"

SpriteComponent::SpriteComponent()
{
}

SpriteComponent::~SpriteComponent()
{
	m_pEnity->GetScene()->GetGame()->GetGraphicsEngine()->RemoveComponent(this);
}

void SpriteComponent::SetSprite(const std::wstring& texPath)
{
	auto pGame = m_pEnity->GetScene()->GetGame();
	// Sprite�̍쐬
	m_pSprite = std::make_shared<Sprite>(pGame);
	m_pSprite->Initialize(texPath);
}

const std::shared_ptr<Sprite>& SpriteComponent::GetSprite()
{
	return m_pSprite;
}

const bool& SpriteComponent::GetActive() const
{
	return m_IsActive;
}

void SpriteComponent::SetActive(const bool& isActive)
{
	m_IsActive = isActive;
}

void SpriteComponent::onCreateInternal()
{
	m_pEnity->GetScene()->GetGame()->GetGraphicsEngine()->AddComponent(this);
}
