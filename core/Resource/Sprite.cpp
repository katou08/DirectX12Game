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

	// リソースバッチを用意
	DirectX::ResourceUploadBatch batch(pDevice);
	batch.Begin();

	DirectX::RenderTargetState rtState(m_pRenderSystem->GetBackBufferFormat(), m_pRenderSystem->GetDepthBufferFormat());

	DirectX::SpriteBatchPipelineStateDescription pd(rtState);

	m_pSpriteBatch = std::make_unique<DirectX::SpriteBatch>(pDevice, batch, pd);

	// ディスクリプタの作成
	m_SpriteHeap.Create(pDevice, "HeapForSprite", D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1);

    // ファイルパスが存在するかどうかチェック
    std::wstring findPath;
    if (!SearchFilePath(texPath.c_str(), findPath))
        return false;

    // ファイル名であることをチェック
    if (PathIsDirectory(findPath.c_str()) != FALSE)
        return false;

    // インスタンスを生成
    m_pTexture = std::make_shared<Texture>();
    if (m_pTexture == nullptr)
        ThrowFailed("テクスチャの生成に失敗.");

    // 初期化
    if (!m_pTexture->Init(pDevice, findPath.c_str(), false, batch, false))
        ThrowFailed("テクスチャの生成に失敗.");

	// テクスチャ用のシェーダーリソースビューを作成
	if (m_pTexture != nullptr)
	{
		auto viewDesc = m_pTexture->GetDesc();
		ShaderResourceView srv = {};
		m_SRV = srv;
		m_SpriteHeap.AllocDescriptor(1, &m_SRV);
		pDevice->CreateShaderResourceView(m_pTexture->GetBuffer(), &viewDesc, m_SRV.GetCPUDescHandle());
	}

	auto future = batch.End(pCmdQUeue); // バッチ終了
	future.wait(); 	// バッチ完了を待機

	// スプライトサイズを設定
	m_SpriteSize = { static_cast<uint32_t>(m_pTexture->GetBuffer()->GetDesc().Width),
	static_cast<uint32_t>(m_pTexture->GetBuffer()->GetDesc().Height) };
	
	// スプライトの位置を設定
	m_SpritePosition = DirectX::XMFLOAT2(25.6f, 25.6f);

	// スプライトの原点を設定
	m_Origin.x = float(m_SpriteSize.x / 2);
	m_Origin.y = float(m_SpriteSize.y / 2);

	return true;
}

void Sprite::Draw()
{
	// 描画前処理
	auto renderContext = m_pRenderSystem->GetRenderContext();
	auto viewport = m_pRenderSystem->GetViewport();
	m_pSpriteBatch->SetViewport(viewport);
	renderContext->SetDescriptorHeap(m_SpriteHeap.GetHeap());

	// 描画処理
	auto cmdList = renderContext->GetCommandList();
	m_pSpriteBatch->Begin(cmdList, DirectX::SpriteSortMode_BackToFront);
	m_pSpriteBatch->Draw(m_SRV.GetGPUDescHandle(), m_SpriteSize, m_SpritePosition, nullptr,
		DirectX::Colors::White, m_Rotation, m_Origin, m_Scale, DirectX::SpriteEffects_None, m_LayerDepth);
	m_pSpriteBatch->End();
}
