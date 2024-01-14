#include "Resource/Text.h"
#include "Game/Game.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/GraphicsEngine.h"
#include "Graphics/RenderContext.h"
#include "Graphics/ResourceHeap.h"

Text::Text(Game* game) : m_pGame(game)
{
}

Text::~Text()
{
}

bool Text::Initialize(const std::wstring& textPath)
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
	m_FontHeap.Create(pDevice, "HeapForFont", D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1);

	// SpriteFontオブジェクトの作成
	m_pSpriteFont = std::make_unique<DirectX::SpriteFont>(pDevice, batch, textPath.c_str(),
		m_FontHeap.GetHeap()->GetCPUDescriptorHandleForHeapStart(),
		m_FontHeap.GetHeap()->GetGPUDescriptorHandleForHeapStart());

	auto future = batch.End(pCmdQUeue); // バッチ終了
	future.wait(); 	// バッチ完了を待機

	// テキスト設定
	textString = L"Hello World";
	return true;
}

void Text::Draw()
{
	// 描画前処理
	auto renderContext = m_pRenderSystem->GetRenderContext();
	auto viewport = m_pRenderSystem->GetViewport();
	m_pSpriteBatch->SetViewport(viewport);
	renderContext->SetDescriptorHeap(m_FontHeap.GetHeap());

	// 描画処理
	auto cmdList = renderContext->GetCommandList();

	DirectX::XMVECTOR result = m_pSpriteFont->MeasureString(textString.c_str());
	m_Origin.x = DirectX::XMVectorGetX(result) / 2.0f;
	m_Origin.y = DirectX::XMVectorGetY(result) / 2.0f;

	m_pSpriteBatch->Begin(cmdList);
	m_pSpriteFont->DrawString(m_pSpriteBatch.get(), textString.c_str(),
		m_TextPosition + Vector2(1.0f, 1.0f), DirectX::Colors::Black, m_Rotation, m_Origin, m_Scale);
	m_pSpriteFont->DrawString(m_pSpriteBatch.get(), textString.c_str(),
		m_TextPosition + Vector2(-1.0f, 1.0f), DirectX::Colors::Black, m_Rotation, m_Origin, m_Scale);
	m_pSpriteFont->DrawString(m_pSpriteBatch.get(), textString.c_str(),
		m_TextPosition + Vector2(1.0f, -1.0f), DirectX::Colors::Black, m_Rotation, m_Origin, m_Scale);
	m_pSpriteFont->DrawString(m_pSpriteBatch.get(), textString.c_str(),
		m_TextPosition + Vector2(-1.0f, -1.0f), DirectX::Colors::Black, m_Rotation, m_Origin, m_Scale);

	m_pSpriteFont->DrawString(m_pSpriteBatch.get(), textString.c_str(), m_TextPosition,
		DirectX::Colors::LightGray, m_Rotation, m_Origin, m_Scale);
	m_pSpriteBatch->End();
}
