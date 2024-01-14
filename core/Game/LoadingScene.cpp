#include "Game/LoadingScene.h"
#include "Game/Scene.h"
#include "Game/Game.h"
#include "Game/Camera.h"
#include "Game/Display.h"
#include "Entity/SpriteComponent.h"
#include "Resource/Sprite.h"
#include "Entity/Entity.h"

LoadingScene::LoadingScene(Game* game, const std::wstring& sceneName)
	: Scene(game, sceneName)
{
}

LoadingScene::~LoadingScene()
{
}

bool LoadingScene::LoadData()
{
	m_pCamera = m_pGame->GetCamera();
	auto display = m_pGame->GetDisplay();
	width = display->getClientSize().width;
	height = display->getClientSize().height;

	m_pSprite = AddEntity<Entity>();
	m_pSpriteComp = m_pSprite->AddComponent<SpriteComponent>();
	m_pSpriteComp->SetSprite(L"res/tex/Loading1.dds");
	m_pSpriteComp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width - 250.0f, height - 100.0f));
	m_pSpriteComp->SetActive(false);

	return true;
}

bool LoadingScene::SetPassive()
{
	m_pSpriteComp->SetActive(false);
	return true;
}

void LoadingScene::Update(float deltaTime)
{
	loadTime += deltaTime;

	OutputDebugStringA(std::to_string(deltaTime).c_str());
	m_pSpriteComp->SetActive(true);
	if (loadTime > 0.5f)
	{
		if (loadSpriteCount == 0)
		{
			m_pSpriteComp->SetSprite(L"res/tex/Loading1.dds");
			m_pSpriteComp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width - 250.0f, height - 100.0f));
			loadTime = 0.0f;
			++loadSpriteCount;
		}
		else if (loadSpriteCount == 1)
		{
			m_pSpriteComp->SetSprite(L"res/tex/Loading2.dds");
			m_pSpriteComp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width - 250.0f, height - 100.0f));
			loadTime = 0.0f;
			++loadSpriteCount;
		}
		else if (loadSpriteCount == 2)
		{
			m_pSpriteComp->SetSprite(L"res/tex/Loading3.dds");
			m_pSpriteComp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width - 250.0f, height - 100.0f));
			loadTime = 0.0f;
			loadSpriteCount = 0;
		}
	}

	Scene::UpdateInternal(deltaTime); //!< シーンクラスのupdate呼び出し
}
