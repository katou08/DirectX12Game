#include "Graphics/GraphicsEngine.h"
#include "Graphics/RenderSystem.h"
#include "System/ImguiSystem.h"
#include "Entity/MeshComponent.h"
#include "Entity/SpriteComponent.h"
#include "Entity/TextComponent.h"
#include "Resource/Mesh.h"
#include "Resource/Sprite.h"
#include "Resource/Text.h"

//! @brief �R���X�g���N�^
//! @param[in] game  �Q�[���N���X
GraphicsEngine::GraphicsEngine(Game* game) : m_pGame(game)
{
	// �����_�[�V�X�e���̏�����
	m_pRenderSystem = std::make_unique<RenderSystem>(game);

	// Imgui�V�X�e���̏�����
	m_pImguiSystem = std::make_unique<ImguiSystem>(game);

}

//! @brief �f�X�g���N�^
GraphicsEngine::~GraphicsEngine()
{
}

//! @brief �������֐�
bool GraphicsEngine::Initialize()
{
	// �����_�[�V�X�e���̏�����
	if (!m_pRenderSystem->Initialize())
		ThrowFailed("�����_�[�V�X�e���̏������Ɏ��s���܂���");

	// Imgui�V�X�e���̏�����
	if (!m_pImguiSystem->Initialize())
		ThrowFailed("Imgui�V�X�e���̏������Ɏ��s���܂���.");

	// Sprite text�p�O���t�B�b�N�X������
	m_pGmemory = std::make_unique<DirectX::GraphicsMemory>(m_pRenderSystem->GetDevice());
	
	return true;
}

//! @brief �X�V�֐�
void GraphicsEngine::Update()
{
#if defined(DEBUG) || defined(_DEBUG)
	m_pImguiSystem->Update();//!< Imgui�̑O����
#endif
	m_pRenderSystem->BeginRender(); //!< �`��̑O����
	

	// ���b�V���̕`��
	for (auto mesh : m_pMeshes)
	{
		if(mesh->GetActive())
			mesh->GetMesh()->Draw();
	}
	// UI�̕`��
	for (auto sprite : m_pSprites)
	{
		if(sprite->GetActive())
			sprite->GetSprite()->Draw();
	}
	// �e�L�X�g�̕`��
	for (auto text : m_pTexts)
	{
		if (text->GetActive())
			text->GetText()->Draw();
	}

#if defined(DEBUG) || defined(_DEBUG)
	m_pImguiSystem->Render(); //!< Imgui�̕`��
#endif
	m_pRenderSystem->EndRender(); //!< �`��̌㏈��

	m_pGmemory->Commit(m_pRenderSystem->GetCmdQueue()); //!< ���ꂪ�Ȃ��ƃ��������[�N���N����
}

//! @brief �R���|�[�l���g�̒ǉ�
//! @param[in] comp �ǉ�����R���|�[�l���g
void GraphicsEngine::AddComponent(Component* comp)
{
	if (auto meshComp = dynamic_cast<MeshComponent*>(comp))
		m_pMeshes.emplace(meshComp);
	else if (auto spriteComp = dynamic_cast<SpriteComponent*>(comp))
		m_pSprites.emplace(spriteComp);
	else if (auto textComp = dynamic_cast<TextComponent*>(comp))
		m_pTexts.emplace(textComp);
}

//! @brief �R���|�[�l���g�̍폜
//! @param[in] comp �폜����R���|�[�l���g
void GraphicsEngine::RemoveComponent(Component* comp)
{
	if (auto meshComp = dynamic_cast<MeshComponent*>(comp))
		m_pMeshes.erase(meshComp);
	else if (auto spriteComp = dynamic_cast<SpriteComponent*>(comp))
		m_pSprites.erase(spriteComp);
	else if (auto textComp = dynamic_cast<TextComponent*>(comp))
		m_pTexts.erase(textComp);
}
