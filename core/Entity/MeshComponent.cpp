#include "Entity/MeshComponent.h"
#include "Resource/Mesh.h"
#include "Entity/Entity.h"
#include "Game/Game.h"
#include "Graphics/GraphicsEngine.h"
#include "System/PhysicsEngine.h"
#include "Game/Scene.h"

MeshComponent::MeshComponent()
{
}

MeshComponent::~MeshComponent()
{
	m_pEnity->GetScene()->GetGame()->GetGraphicsEngine()->RemoveComponent(this);
}

void MeshComponent::SetMesh(const std::wstring& meshPath, const std::wstring& vsPath, const std::wstring& psPath)
{
	auto pGame = m_pEnity->GetScene()->GetGame();
	// ƒƒbƒVƒ…‚Ìì¬
	m_pMesh = std::make_shared<Mesh>(pGame);

	if (vsPath != L"" && psPath != L"")
		m_pMesh->Init(meshPath, vsPath, psPath);
	else
		m_pMesh->Init(meshPath);
}

const std::shared_ptr<Mesh>& MeshComponent::GetMesh()
{
	return m_pMesh;
}

const bool& MeshComponent::GetActive() const
{
	return m_IsActive;
}

void MeshComponent::SetActive(const bool& isActive)
{
	m_IsActive = isActive;
}

void MeshComponent::onCreateInternal()
{
	m_pEnity->GetScene()->GetGame()->GetGraphicsEngine()->AddComponent(this);
}
