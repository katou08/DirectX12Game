#include "Graphics/GraphicsEngine.h"
#include "Graphics/RenderSystem.h"
#include "System/ImguiSystem.h"
#include "Entity/MeshComponent.h"
#include "Entity/SpriteComponent.h"
#include "Entity/TextComponent.h"
#include "Resource/Mesh.h"
#include "Resource/Sprite.h"
#include "Resource/Text.h"

//! @brief コンストラクタ
//! @param[in] game  ゲームクラス
GraphicsEngine::GraphicsEngine(Game* game) : m_pGame(game)
{
	// レンダーシステムの初期化
	m_pRenderSystem = std::make_unique<RenderSystem>(game);

	// Imguiシステムの初期化
	m_pImguiSystem = std::make_unique<ImguiSystem>(game);

}

//! @brief デストラクタ
GraphicsEngine::~GraphicsEngine()
{
}

//! @brief 初期化関数
bool GraphicsEngine::Initialize()
{
	// レンダーシステムの初期化
	if (!m_pRenderSystem->Initialize())
		ThrowFailed("レンダーシステムの初期化に失敗しました");

	// Imguiシステムの初期化
	if (!m_pImguiSystem->Initialize())
		ThrowFailed("Imguiシステムの初期化に失敗しました.");

	// Sprite text用グラフィックスメモリ
	m_pGmemory = std::make_unique<DirectX::GraphicsMemory>(m_pRenderSystem->GetDevice());
	
	return true;
}

//! @brief 更新関数
void GraphicsEngine::Update()
{
#if defined(DEBUG) || defined(_DEBUG)
	m_pImguiSystem->Update();//!< Imguiの前処理
#endif
	m_pRenderSystem->BeginRender(); //!< 描画の前処理
	

	// メッシュの描画
	for (auto mesh : m_pMeshes)
	{
		if(mesh->GetActive())
			mesh->GetMesh()->Draw();
	}
	// UIの描画
	for (auto sprite : m_pSprites)
	{
		if(sprite->GetActive())
			sprite->GetSprite()->Draw();
	}
	// テキストの描画
	for (auto text : m_pTexts)
	{
		if (text->GetActive())
			text->GetText()->Draw();
	}

#if defined(DEBUG) || defined(_DEBUG)
	m_pImguiSystem->Render(); //!< Imguiの描画
#endif
	m_pRenderSystem->EndRender(); //!< 描画の後処理

	m_pGmemory->Commit(m_pRenderSystem->GetCmdQueue()); //!< これがないとメモリリークを起こす
}

//! @brief コンポーネントの追加
//! @param[in] comp 追加するコンポーネント
void GraphicsEngine::AddComponent(Component* comp)
{
	if (auto meshComp = dynamic_cast<MeshComponent*>(comp))
		m_pMeshes.emplace(meshComp);
	else if (auto spriteComp = dynamic_cast<SpriteComponent*>(comp))
		m_pSprites.emplace(spriteComp);
	else if (auto textComp = dynamic_cast<TextComponent*>(comp))
		m_pTexts.emplace(textComp);
}

//! @brief コンポーネントの削除
//! @param[in] comp 削除するコンポーネント
void GraphicsEngine::RemoveComponent(Component* comp)
{
	if (auto meshComp = dynamic_cast<MeshComponent*>(comp))
		m_pMeshes.erase(meshComp);
	else if (auto spriteComp = dynamic_cast<SpriteComponent*>(comp))
		m_pSprites.erase(spriteComp);
	else if (auto textComp = dynamic_cast<TextComponent*>(comp))
		m_pTexts.erase(textComp);
}
