#include "Entity/TextComponent.h"
#include "Resource/Text.h"
#include "Entity/Entity.h"
#include "Game/Game.h"
#include "Graphics/GraphicsEngine.h"
#include "Game/Scene.h"

TextComponent::TextComponent()
{
}

TextComponent::~TextComponent()
{
	m_pEnity->GetScene()->GetGame()->GetGraphicsEngine()->RemoveComponent(this);
}

void TextComponent::SetText(const std::wstring& textPath)
{
	auto pGame = m_pEnity->GetScene()->GetGame();
	// Text�̍쐬
	m_pText = std::make_shared<Text>(pGame);
	m_pText->Initialize(textPath);
}

const std::shared_ptr<Text>& TextComponent::GetText()
{
	return m_pText;
}

const bool& TextComponent::GetActive() const
{
	return m_IsActive;
}

void TextComponent::SetActive(const bool& isActive)
{
	m_IsActive = isActive;
}

void TextComponent::onCreateInternal()
{
	m_pEnity->GetScene()->GetGame()->GetGraphicsEngine()->AddComponent(this);
}
