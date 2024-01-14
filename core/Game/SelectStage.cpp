#include "Game/SelectStage.h"
#include "Game/Game.h"
#include "System/AudioSystem.h"
#include "System/InputSystem.h"
#include "Game/Camera.h"
#include "Entity/Entity.h"
#include "Entity/Component.h"
#include "Entity/TransformComponent.h"
#include "Entity/SpriteComponent.h"
#include "Resource/Sprite.h"
#include "Entity/TextComponent.h"
#include "Resource/Text.h"
#include "Game/Display.h"

SelectStage::SelectStage(Game* game, const std::wstring& sceneName)
	: Scene(game, sceneName)
{
}

SelectStage::~SelectStage()
{
}

bool SelectStage::LoadData()
{
	m_pCamera = m_pGame->GetCamera();
	m_pCamera->Reset();
	auto display = m_pGame->GetDisplay();
	width = display->getClientSize().width;
	height = display->getClientSize().height;

	GenerateCubes();

	m_pGame->GetAudioSystem()->PlayBGM(L"sound/selectStage.wav", 0.1f, 0.0f, 0.0f, true);

	m_pSpriteStage1 = AddEntity<Entity>();
	m_pSpriteStage1Comp = m_pSpriteStage1->AddComponent<SpriteComponent>();
	m_pSpriteStage1Comp->SetSprite(L"res/tex/SelectStageButton.dds");
	m_pSpriteStage1Comp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2 - 500.0f, height / 2 - 100.0f));
	m_pSpriteStage1Comp->GetSprite()->SetLayerDepth(0.0f);

	m_pSpriteStage2 = AddEntity<Entity>();
	m_pSpriteStage2Comp = m_pSpriteStage2->AddComponent<SpriteComponent>();
	m_pSpriteStage2Comp->SetSprite(L"res/tex/SelectStageButton.dds");
	m_pSpriteStage2Comp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2, height / 2 - 100.0f));
	m_pSpriteStage2Comp->GetSprite()->SetLayerDepth(0.0f);

	m_pSpriteStage3 = AddEntity<Entity>();
	m_pSpriteStage3Comp = m_pSpriteStage3->AddComponent<SpriteComponent>();
	m_pSpriteStage3Comp->SetSprite(L"res/tex/SelectStageButton.dds");
	m_pSpriteStage3Comp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2 + 500.0f, height / 2 - 100.0f));
	m_pSpriteStage3Comp->GetSprite()->SetLayerDepth(0.0f);

	m_pSpriteSelectedStage1 = AddEntity<Entity>();
	m_pSpriteSelectedStage1Comp = m_pSpriteSelectedStage1->AddComponent<SpriteComponent>();
	m_pSpriteSelectedStage1Comp->SetSprite(L"res/tex/SelectedStageButton.dds");
	m_pSpriteSelectedStage1Comp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2 - 500.0f, height / 2 - 100.0f));
	m_pSpriteSelectedStage1Comp->SetActive(false);
	m_pSpriteSelectedStage1Comp->GetSprite()->SetLayerDepth(1.0f);

	m_pSpriteSelectedStage2 = AddEntity<Entity>();
	m_pSpriteSelectedStage2Comp = m_pSpriteSelectedStage2->AddComponent<SpriteComponent>();
	m_pSpriteSelectedStage2Comp->SetSprite(L"res/tex/SelectedStageButton.dds");
	m_pSpriteSelectedStage2Comp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2, height / 2 - 100.0f));
	m_pSpriteSelectedStage2Comp->GetSprite()->SetLayerDepth(1.0f);
	m_pSpriteSelectedStage2Comp->SetActive(false);

	m_pSpriteSelectedStage3 = AddEntity<Entity>();
	m_pSpriteSelectedStage3Comp = m_pSpriteSelectedStage3->AddComponent<SpriteComponent>();
	m_pSpriteSelectedStage3Comp->SetSprite(L"res/tex/SelectedStageButton.dds");
	m_pSpriteSelectedStage3Comp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2 + 500.0f, height / 2 - 100.0f));
	m_pSpriteSelectedStage3Comp->SetActive(false);
	m_pSpriteSelectedStage3Comp->GetSprite()->SetLayerDepth(1.0f);
	
	m_pSpriteTitle = AddEntity<Entity>();
	m_pSpriteTitleComp = m_pSpriteTitle->AddComponent<SpriteComponent>();
	m_pSpriteTitleComp->SetSprite(L"res/tex/SelectButton.dds");
	m_pSpriteTitleComp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2 - 800.0f, height / 2 - 100.0f));

	m_pSpriteSelectedTitle = AddEntity<Entity>();
	m_pSpriteSelectedTitleComp = m_pSpriteSelectedTitle->AddComponent<SpriteComponent>();
	m_pSpriteSelectedTitleComp->SetSprite(L"res/tex/SelectedButton_Gray.dds");
	m_pSpriteSelectedTitleComp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2, height / 2 + 400.0f));
	m_pSpriteSelectedTitleComp->SetActive(false);

	m_pFontSelectStage = AddEntity<Entity>();
	m_pFontSelectStageComp = m_pFontSelectStage->AddComponent<TextComponent>();
	m_pFontSelectStageComp->SetText(L"res/font/fonttest.spritefont");
	m_pFontSelectStageComp->GetText()->SetTextString(L"STAGE SELECT");
	m_pFontSelectStageComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, height / 2 - 450.0f));
	m_pFontSelectStageComp->GetText()->SetTextScale(1.0f);

	m_pFontTitle = AddEntity<Entity>();
	m_pFontTitleComp = m_pFontTitle->AddComponent<TextComponent>();
	m_pFontTitleComp->SetText(L"res/font/fonttest.spritefont");
	m_pFontTitleComp->GetText()->SetTextString(L"Back To Title");
	m_pFontTitleComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, height / 2 + 400.0f));

	m_pSpriteSelectArrow = AddEntity<Entity>();
	m_pSpriteSelectArrowComp = m_pSpriteSelectArrow->AddComponent<SpriteComponent>();
	m_pSpriteSelectArrowComp->SetSprite(L"res/tex/SelectArrow.dds");
	m_pSpriteSelectArrowComp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(100.0f, height - 100.0f));
	m_pSpriteSelectArrowComp->GetSprite()->SetSpriteScale(0.3f);
	
	m_pSpriteEnterArrow = AddEntity<Entity>();
	m_pSpriteEnterComp = m_pSpriteEnterArrow->AddComponent<SpriteComponent>();
	m_pSpriteEnterComp->SetSprite(L"res/tex/Enter.dds");
	m_pSpriteEnterComp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(250.0f, height - 100.0f));
	m_pSpriteEnterComp->GetSprite()->SetSpriteScale(0.2f);

	m_pFontSelect = AddEntity<Entity>();
	m_pFontSelectComp = m_pFontSelect->AddComponent<TextComponent>();
	m_pFontSelectComp->SetText(L"res/font/fonttest.spritefont");
	m_pFontSelectComp->GetText()->SetTextString(L"Select");
	m_pFontSelectComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(100.0f, height - 25.0f));
	m_pFontSelectComp->GetText()->SetTextScale(0.5f);

	m_pFontDecision = AddEntity<Entity>();
	m_pFontDecisionComp = m_pFontDecision->AddComponent<TextComponent>();
	m_pFontDecisionComp->SetText(L"res/font/fonttest.spritefont");
	m_pFontDecisionComp->GetText()->SetTextString(L"Select Button");
	m_pFontDecisionComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(250.0f, height - 25.0f));
	m_pFontDecisionComp->GetText()->SetTextScale(0.5f);
	
	m_pFontStage1 = AddEntity<Entity>();
	m_pFontStage1Comp = m_pFontStage1->AddComponent<TextComponent>();
	m_pFontStage1Comp->SetText(L"res/font/fonttest.spritefont");
	m_pFontStage1Comp->GetText()->SetTextString(L"Stage1");
	m_pFontStage1Comp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2 - 500, height / 2 + 300.0f));
	m_pFontStage1Comp->GetText()->SetTextScale(0.75f);
	
	m_pFontStage2 = AddEntity<Entity>();
	m_pFontStage2Comp = m_pFontStage2->AddComponent<TextComponent>();
	m_pFontStage2Comp->SetText(L"res/font/fonttest.spritefont");
	m_pFontStage2Comp->GetText()->SetTextString(L"Stage2");
	m_pFontStage2Comp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, height / 2 + 300.0f));
	m_pFontStage2Comp->GetText()->SetTextScale(0.75f);

	m_pFontStage3 = AddEntity<Entity>();
	m_pFontStage3Comp = m_pFontStage3->AddComponent<TextComponent>();
	m_pFontStage3Comp->SetText(L"res/font/fonttest.spritefont");
	m_pFontStage3Comp->GetText()->SetTextString(L"Stage3");
	m_pFontStage3Comp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2 + 500, height / 2 + 300.0f));
	m_pFontStage3Comp->GetText()->SetTextScale(0.75f);

	loadTime = 0.0f;

	return true;
}

void SelectStage::GenerateCubes()
{
	m_pCubes = AddEntity<Entity>();
	m_pCubesMesh = m_pCubes->AddComponent<MeshComponent>();
	m_pCubesMesh->SetMesh(L"res/box/box.obj");
	m_pCubesMesh->GetMesh()->ChangeInstanceCount(500);
	m_Axis.resize(500);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> distributionx(-25.0f, 25.0f);
	std::uniform_real_distribution<float> distributiony(-15.0f, 15.0f);
	std::uniform_real_distribution<float> distributionz(10.0f, 50.0f);
	std::uniform_real_distribution<float> distributionAxisX(0.0f, 1.0f);
	std::uniform_real_distribution<float> distributionAxisY(0.0f, 1.0f);
	std::uniform_real_distribution<float> distributionAxisZ(0.0f, 1.0f);
	std::uniform_real_distribution<float> distributionScale(0.1f, 0.2f);
	for (int i = 0; i < m_pCubesMesh->GetMesh()->GetInstanceCount(); ++i)
	{
		float AxisX = distributionAxisX(gen);
		float AxisY = distributionAxisY(gen);
		float AxisZ = distributionAxisZ(gen);
		float randomNumberx = distributionx(gen);
		float randomNumbery = distributiony(gen);
		float randomNumberz = distributionz(gen);
		float randScale = distributionScale(gen);
		m_pCubesMesh->GetMesh()->SetPosition(Vector3(randomNumberx, randomNumbery, randomNumberz), i);
		m_pCubesMesh->GetMesh()->SetScale(Vector3(randScale, randScale, randScale), i);
		m_Axis[i] = { AxisX, AxisY, AxisZ };
	}
}

void SelectStage::Update(float deltaTime)
{
	auto display = m_pGame->GetDisplay();
	width = display->getClientSize().width;
	height = display->getClientSize().height;

	UpdatePosition();

	UpdateCubes(deltaTime);

	loadTime += deltaTime;
	selectDelayTime += deltaTime;
	auto kb = m_pGame->GetInputSystem()->GetKeybord()->GetState();
	auto& kbLog = m_pGame->GetInputSystem()->GetKeyLog();

	if (kb.Left)
	{
		if (selectDelayTime > 0.1f)
		{
			if (selectTitleCount == 0)
			{
				--selectCount;
				if (selectCount <= 0)
					selectCount = 0;
			}
			selectDelayTime = 0.0f;
		}
	}
	else if (kb.Right)
	{
		if (selectDelayTime > 0.1f)
		{
			if (selectTitleCount == 0)
			{
				++selectCount;
				if (selectCount >= 2)
					selectCount = 2;
			}
			selectDelayTime = 0.0f;
		}
	}
	else if (kb.Up)
	{
		if (selectDelayTime > 0.1f)
		{
			--selectTitleCount;
			if (selectTitleCount <= 0)
				selectTitleCount = 0;
		}
		selectDelayTime = 0.0f;
	}
	else if (kb.Down)
	{
		if (selectDelayTime > 0.1f)
		{
			++selectTitleCount;
			if (selectTitleCount >= 1)
				selectTitleCount = 1;
		}
		selectDelayTime = 0.0f;
	}

	if (selectTitleCount == 0)
	{
		m_pSpriteSelectedTitleComp->SetActive(false);
		if (selectTitleAudioCount == 1)
		{
			m_pGame->GetAudioSystem()->PlaySE(L"sound/select.wav", 0.1f);
		}
		selectTitleAudioCount = 0;
		if (selectCount == 0)
		{
			if (selectAudioCount == 1)
			{
				m_pGame->GetAudioSystem()->PlaySE(L"sound/select.wav", 0.1f);
			}
			selectAudioCount = 0;

			m_pSpriteStage1Comp->SetActive(false);
			m_pSpriteStage2Comp->SetActive(true);
			m_pSpriteStage3Comp->SetActive(true);
			m_pSpriteSelectedStage1Comp->SetActive(true);
			m_pSpriteSelectedStage2Comp->SetActive(false);
			m_pSpriteSelectedStage3Comp->SetActive(false);

			if (kbLog.pressed.Enter)
			{
				m_pGame->GetAudioSystem()->StopBGM(L"sound/selectStage.wav");
				m_pGame->GetAudioSystem()->PlaySE(L"sound/click.wav", 0.1f);
				if (loadTime > 0.5f)
				{
					// ステージシーンをロード
					IsQuit = true;
					m_NextSceneName = L"Stage";
				}
			}
		}
		else if (selectCount == 1)
		{
			if (selectAudioCount == 0)
			{
				m_pGame->GetAudioSystem()->PlaySE(L"sound/select.wav", 0.1f);
			}
			selectAudioCount = 1;

			m_pSpriteStage1Comp->SetActive(true);
			m_pSpriteStage2Comp->SetActive(false);
			m_pSpriteStage3Comp->SetActive(true);
			m_pSpriteSelectedStage1Comp->SetActive(false);
			m_pSpriteSelectedStage2Comp->SetActive(true);
			m_pSpriteSelectedStage3Comp->SetActive(false);

			if (kbLog.pressed.Enter)
			{
				m_pGame->GetAudioSystem()->StopBGM(L"sound/selectStage.wav");
				m_pGame->GetAudioSystem()->PlaySE(L"sound/click.wav", 0.1f);
				if (loadTime > 0.5f)
				{
					// ステージシーンをロード
					IsQuit = true;
					m_NextSceneName = L"Stage2";
				}
			}
		}
		else if (selectCount == 2)
		{
			if (selectAudioCount == 1)
			{
				m_pGame->GetAudioSystem()->PlaySE(L"sound/select.wav", 0.1f);
			}
			selectAudioCount = 0;

			m_pSpriteStage1Comp->SetActive(true);
			m_pSpriteStage2Comp->SetActive(true);
			m_pSpriteStage3Comp->SetActive(false);
			m_pSpriteSelectedStage1Comp->SetActive(false);
			m_pSpriteSelectedStage2Comp->SetActive(false);
			m_pSpriteSelectedStage3Comp->SetActive(true);

			if (kbLog.pressed.Enter)
			{
				m_pGame->GetAudioSystem()->StopBGM(L"sound/selectStage.wav");
				m_pGame->GetAudioSystem()->PlaySE(L"sound/click.wav", 0.1f);
				if (loadTime > 0.5f)
				{
					// ステージシーンをロード
					IsQuit = true;
					m_NextSceneName = L"Stage3";
				}
			}
		}
	}
	else if(selectTitleCount == 1)
	{
		if (selectTitleAudioCount == 0)
		{
			m_pGame->GetAudioSystem()->PlaySE(L"sound/select.wav", 0.1f);
		}
		selectTitleAudioCount = 1;
		m_pSpriteStage1Comp->SetActive(true);
		m_pSpriteStage2Comp->SetActive(true);
		m_pSpriteStage3Comp->SetActive(true);
		m_pSpriteSelectedStage1Comp->SetActive(false);
		m_pSpriteSelectedStage2Comp->SetActive(false);
		m_pSpriteSelectedStage3Comp->SetActive(false);
		m_pSpriteSelectedTitleComp->SetActive(true);

		if (kbLog.pressed.Enter)
		{
			m_pGame->GetAudioSystem()->StopBGM(L"sound/selectStage.wav");
			m_pGame->GetAudioSystem()->PlaySE(L"sound/click.wav", 0.1f);
			if (loadTime > 0.5f)
			{
				// ステージシーンをロード
				IsQuit = true;
				m_NextSceneName = L"Title";
			}
		}
	}

	Scene::UpdateInternal(deltaTime); //!< シーンクラスのupdate呼び出し
}

void SelectStage::UpdateCubes(float deltaTime)
{
	m_addDeltaTime += deltaTime;
	for (int i = 0; i < m_pCubesMesh->GetMesh()->GetInstanceCount(); ++i)
	{
		auto q = Quaternion::CreateFromAxisAngle(m_Axis[i], DirectX::XM_PIDIV2 * m_addDeltaTime * 0.5f);
		m_pCubesMesh->GetMesh()->SetRotation(q, i);
	}
}

void SelectStage::UpdatePosition()
{
	m_pSpriteStage1Comp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2 - 500.0f, height / 2 - 100.0f));
	m_pSpriteStage2Comp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2, height / 2 - 100.0f));
	m_pSpriteStage3Comp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2 + 500.0f, height / 2 - 100.0f));
	m_pSpriteSelectedStage1Comp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2 - 500.0f, height / 2 - 100.0f));
	m_pSpriteSelectedStage2Comp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2, height / 2 - 100.0f));
	m_pSpriteSelectedStage3Comp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2 + 500.0f, height / 2 - 100.0f));
	m_pSpriteTitleComp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2 - 800.0f, height / 2 - 100.0f));
	m_pSpriteSelectedTitleComp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2, height / 2 + 400.0f));
	m_pFontSelectStageComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, height / 2 - 450.0f));
	m_pFontTitleComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, height / 2 + 400.0f));
	m_pSpriteSelectArrowComp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(100.0f, height - 100.0f));
	m_pSpriteEnterComp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(250.0f, height - 100.0f));
	m_pFontSelectComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(100.0f, height - 25.0f));
	m_pFontDecisionComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(250.0f, height - 25.0f));
	m_pFontStage1Comp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2 - 500, height / 2 + 300.0f));
	m_pFontStage2Comp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, height / 2 + 300.0f));
	m_pFontStage3Comp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2 + 500, height / 2 + 300.0f));
}
