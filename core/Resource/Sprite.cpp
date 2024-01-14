#include "Resource/Sprite.h"
#include "Game/Game.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/GraphicsEngine.h"
#include "Graphics/RenderContext.h"
#include "Graphics/Texture.h"

Sprite::Sprite(Game* game) : m_pGame(game)
{
}

Sprite::~Sprite()
{
}

bool Sprite::Initialize(const std::wstring texPath)
{
	m_pRenderSystem = m_pGame->GetGraphicsEngine()->GetRenderSystem();
	auto pDevice = m_pRenderSystem->GetDevice();
	auto pCmdQUeue = m_pRenderSystem->GetCmdQueue();

	// ���\�[�X�o�b�`��p��
	DirectX::ResourceUploadBatch batch(pDevice);
	batch.Begin();

	DirectX::RenderTargetState rtState(m_pRenderSystem->GetBackBufferFormat(), m_pRenderSystem->GetDepthBufferFormat());

	DirectX::SpriteBatchPipelineStateDescription pd(rtState);

	m_pSpriteBatch = std::make_unique<DirectX::SpriteBatch>(pDevice, batch, pd);

	// �f�B�X�N���v�^�̍쐬
	m_SpriteHeap.Create(pDevice, "HeapForSprite", D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1);

    // �t�@�C���p�X�����݂��邩�ǂ����`�F�b�N
    std::wstring findPath;
    if (!SearchFilePath(texPath.c_str(), findPath))
        return false;

    // �t�@�C�����ł��邱�Ƃ��`�F�b�N
    if (PathIsDirectory(findPath.c_str()) != FALSE)
        return false;

    // �C���X�^���X�𐶐�
    m_pTexture = std::make_shared<Texture>();
    if (m_pTexture == nullptr)
        ThrowFailed("�e�N�X�`���̐����Ɏ��s.");

    // ������
    if (!m_pTexture->Init(pDevice, findPath.c_str(), false, batch, false))
        ThrowFailed("�e�N�X�`���̐����Ɏ��s.");

	// �e�N�X�`���p�̃V�F�[�_�[���\�[�X�r���[���쐬
	if (m_pTexture != nullptr)
	{
		auto viewDesc = m_pTexture->GetDesc();
		ShaderResourceView srv = {};
		m_SRV = srv;
		m_SpriteHeap.AllocDescriptor(1, &m_SRV);
		pDevice->CreateShaderResourceView(m_pTexture->GetBuffer(), &viewDesc, m_SRV.GetCPUDescHandle());
	}

	auto future = batch.End(pCmdQUeue); // �o�b�`�I��
	future.wait(); 	// �o�b�`������ҋ@

	// �X�v���C�g�T�C�Y��ݒ�
	m_SpriteSize = { static_cast<uint32_t>(m_pTexture->GetBuffer()->GetDesc().Width),
	static_cast<uint32_t>(m_pTexture->GetBuffer()->GetDesc().Height) };
	
	// �X�v���C�g�̈ʒu��ݒ�
	m_SpritePosition = DirectX::XMFLOAT2(25.6f, 25.6f);

	// �X�v���C�g�̌��_��ݒ�
	m_Origin.x = float(m_SpriteSize.x / 2);
	m_Origin.y = float(m_SpriteSize.y / 2);

	return true;
}

void Sprite::Draw()
{
	// �`��O����
	auto renderContext = m_pRenderSystem->GetRenderContext();
	auto viewport = m_pRenderSystem->GetViewport();
	m_pSpriteBatch->SetViewport(viewport);
	renderContext->SetDescriptorHeap(m_SpriteHeap.GetHeap());

	// �`�揈��
	auto cmdList = renderContext->GetCommandList();
	m_pSpriteBatch->Begin(cmdList, DirectX::SpriteSortMode_BackToFront);
	m_pSpriteBatch->Draw(m_SRV.GetGPUDescHandle(), m_SpriteSize, m_SpritePosition, nullptr,
		DirectX::Colors::White, m_Rotation, m_Origin, m_Scale, DirectX::SpriteEffects_None, m_LayerDepth);
	m_pSpriteBatch->End();
}
