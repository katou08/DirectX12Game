#include "Game/ResultScreen.h"
#include "Entity/TextComponent.h"
#include "Entity/SpriteComponent.h"
#include "Resource/Sprite.h"
#include "Resource/Text.h"
#include "Game/Game.h"
#include "Game/Scene.h"
#include "Game/Display.h"
#include "System/InputSystem.h"
#include "System/AudioSystem.h"

ResultScreen::ResultScreen()
{
}

ResultScreen::~ResultScreen()
{
}

void ResultScreen::OnCreate()
{
	Entity::OnCreate();

	auto display = m_pScene->GetGame()->GetDisplay();
	auto width = display->getClientSize().width;
	auto height = display->getClientSize().height;

	m_pEntities.resize(19);
	for (int i = 0; i < m_pEntities.size(); ++i)
		m_pEntities[i] = m_pScene->AddEntity<Entity>();

	m_pBackGroundSpriteComp = m_pEntities[0]->AddComponent<SpriteComponent>();
	m_pBackGroundSpriteComp->SetSprite(L"res/tex/ResultBackground.dds");
	m_pBackGroundSpriteComp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2, height / 2));
	m_pBackGroundSpriteComp->SetActive(false);

	m_pSelectSpriteComp = m_pEntities[1]->AddComponent<SpriteComponent>();
	m_pSelectSpriteComp->SetSprite(L"res/tex/SelectButton.dds");
	m_pSelectSpriteComp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2, height / 2 + 100.0f));
	m_pSelectSpriteComp->SetActive(false);

	m_pSelectedSpriteComp = m_pEntities[2]->AddComponent<SpriteComponent>();
	m_pSelectedSpriteComp->SetSprite(L"res/tex/SelectedButton_Gray.dds");
	m_pSelectedSpriteComp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2, height / 2 + 100.0f));
	m_pSelectedSpriteComp->SetActive(false);

	m_pSelectSpriteComp1 = m_pEntities[3]->AddComponent<SpriteComponent>();
	m_pSelectSpriteComp1->SetSprite(L"res/tex/SelectButton.dds");
	m_pSelectSpriteComp1->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2, height / 2 + 200.0f));
	m_pSelectSpriteComp1->SetActive(false);

	m_pSelectedSpriteComp1 = m_pEntities[4]->AddComponent<SpriteComponent>();
	m_pSelectedSpriteComp1->SetSprite(L"res/tex/SelectedButton_Gray.dds");
	m_pSelectedSpriteComp1->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2, height / 2 + 200.0f));
	m_pSelectedSpriteComp1->SetActive(false);

	m_pSelectSpriteComp2 = m_pEntities[5]->AddComponent<SpriteComponent>();
	m_pSelectSpriteComp2->SetSprite(L"res/tex/SelectButton.dds");
	m_pSelectSpriteComp2->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2, height / 2 + 300.0f));
	m_pSelectSpriteComp2->SetActive(false);

	m_pSelectedSpriteComp2 = m_pEntities[6]->AddComponent<SpriteComponent>();
	m_pSelectedSpriteComp2->SetSprite(L"res/tex/SelectedButton_Gray.dds");
	m_pSelectedSpriteComp2->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2, height / 2 + 300.0f));
	m_pSelectedSpriteComp2->SetActive(false);

	m_pMoveSpriteComp = m_pEntities[7]->AddComponent<SpriteComponent>();
	m_pMoveSpriteComp->SetSprite(L"res/tex/Wasd.dds");
	m_pMoveSpriteComp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(100.0f, height - 100.0f));
	m_pMoveSpriteComp->GetSprite()->SetSpriteScale(0.2f);
	m_pMoveSpriteComp->SetActive(false);

	m_pLeftClickSpriteComp = m_pEntities[8]->AddComponent<SpriteComponent>();
	m_pLeftClickSpriteComp->SetSprite(L"res/tex/leftClickWhite.dds");
	m_pLeftClickSpriteComp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(250.0f, height - 100.0f));
	m_pLeftClickSpriteComp->GetSprite()->SetSpriteScale(0.2f);
	m_pLeftClickSpriteComp->SetActive(false);

	m_pClearTextComp = m_pEntities[9]->AddComponent<TextComponent>();
	m_pClearTextComp->SetText(L"res/font/fonttest.spritefont");
	m_pClearTextComp->GetText()->SetTextString(L"CLEAR");
	m_pClearTextComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, height / 2 - 250.0f));
	m_pClearTextComp->SetActive(false);

	m_pGameOverTextComp = m_pEntities[10]->AddComponent<TextComponent>();
	m_pGameOverTextComp->SetText(L"res/font/fonttest.spritefont");
	m_pGameOverTextComp->GetText()->SetTextString(L"GAME OVER");
	m_pGameOverTextComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, height / 2 - 250.0f));
	m_pGameOverTextComp->SetActive(false);

	m_pPauseTextComp = m_pEntities[11]->AddComponent<TextComponent>();
	m_pPauseTextComp->SetText(L"res/font/fonttest.spritefont");
	m_pPauseTextComp->GetText()->SetTextString(L"PAUSE");
	m_pPauseTextComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, height / 2 - 50.0f));
	m_pPauseTextComp->SetActive(false);

	m_pTotalShotTextComp = m_pEntities[12]->AddComponent<TextComponent>();
	m_pTotalShotTextComp->SetText(L"res/font/fonttest.spritefont");
	m_pTotalShotTextComp->GetText()->SetTextString(L"TOTAL SHOT : " + std::to_wstring(totalFireBullets));
	m_pTotalShotTextComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, height / 2 - 150.0f));
	m_pTotalShotTextComp->SetActive(false);

	m_pRestartTextComp = m_pEntities[13]->AddComponent<TextComponent>();
	m_pRestartTextComp->SetText(L"res/font/fonttest.spritefont");
	m_pRestartTextComp->GetText()->SetTextString(L"RESTART");
	m_pRestartTextComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, height / 2 + 100.0f));
	m_pRestartTextComp->SetActive(false);

	m_pResumeTextComp = m_pEntities[14]->AddComponent<TextComponent>();
	m_pResumeTextComp->SetText(L"res/font/fonttest.spritefont");
	m_pResumeTextComp->GetText()->SetTextString(L"RESUME");
	m_pResumeTextComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, height / 2 + 100.0f));
	m_pResumeTextComp->SetActive(false);

	m_pStageSelectComp = m_pEntities[15]->AddComponent<TextComponent>();
	m_pStageSelectComp->SetText(L"res/font/fonttest.spritefont");
	m_pStageSelectComp->GetText()->SetTextString(L"STAGE SELECT");
	m_pStageSelectComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, height / 2 + 200.0f));
	m_pStageSelectComp->SetActive(false);

	m_pTitleTextComp = m_pEntities[16]->AddComponent<TextComponent>();
	m_pTitleTextComp->SetText(L"res/font/fonttest.spritefont");
	m_pTitleTextComp->GetText()->SetTextString(L"TITLE");
	m_pTitleTextComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, height / 2 + 300.0f));
	m_pTitleTextComp->SetActive(false);

	m_pFontMoveTextComp = m_pEntities[17]->AddComponent<TextComponent>();
	m_pFontMoveTextComp->SetText(L"res/font/fonttest.spritefont");
	m_pFontMoveTextComp->GetText()->SetTextString(L"MOVE");
	m_pFontMoveTextComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(100.0f, height - 25.0f));
	m_pFontMoveTextComp->GetText()->SetTextScale(0.5f);
	m_pFontMoveTextComp->SetActive(false);

	m_pFontFireTextComp = m_pEntities[18]->AddComponent<TextComponent>();
	m_pFontFireTextComp->SetText(L"res/font/fonttest.spritefont");
	m_pFontFireTextComp->GetText()->SetTextString(L"FIRE");
	m_pFontFireTextComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(250.0f, height - 25.0f));
	m_pFontFireTextComp->GetText()->SetTextScale(0.5f);
	m_pFontFireTextComp->SetActive(false);
}

void ResultScreen::OnUpdate(float deltaTime)
{
	Entity::OnUpdate(deltaTime);

	auto display = m_pScene->GetGame()->GetDisplay();
	auto width = display->getClientSize().width;
	auto height = display->getClientSize().height;

	UpdatePosition(width, height);

	totalTime += deltaTime;
	selectDelayTime += deltaTime;

	if (m_pScene->GetIsPause())
	{
		++limitCount;
		if(limitCount == 1)
			showTotalTime = totalTime;

		isPause = true;

		m_pBackGroundSpriteComp->SetActive(true);
		m_pSelectSpriteComp->SetActive(true);
		m_pSelectedSpriteComp->SetActive(true);
		m_pSelectSpriteComp1->SetActive(true);
		m_pSelectSpriteComp2->SetActive(true);
		m_pLeftClickSpriteComp->SetActive(true);
		m_pMoveSpriteComp->SetActive(true);
		m_pClearTextComp->SetActive(false);
		m_pPauseTextComp->SetActive(true);
		m_pGameOverTextComp->SetActive(false);
		m_pTotalShotTextComp->GetText()->SetTextString(L"TOTAL SHOT : " + std::to_wstring(totalFireBullets));
		m_pTotalShotTextComp->SetActive(false);
		m_pRestartTextComp->SetActive(false);
		m_pResumeTextComp->SetActive(true);
		m_pTitleTextComp->SetActive(true);
		m_pStageSelectComp->SetActive(true);
		m_pFontMoveTextComp->SetActive(true);
		m_pFontFireTextComp->SetActive(true);

		auto kb = GetScene()->GetGame()->GetInputSystem()->GetKeybord()->GetState();
		auto& kbLog = GetScene()->GetGame()->GetInputSystem()->GetKeyLog();

		if (kb.Up)
		{
			if (selectDelayTime > 0.1f)
			{
				--selectCount;
				if (selectCount <= 0)
					selectCount = 0;
				selectDelayTime = 0.0f;
			}
		}
		else if (kb.Down)
		{
			if (selectDelayTime > 0.1f)
			{
				++selectCount;
				if (selectCount >= 2)
					selectCount = 2;
				selectDelayTime = 0.0f;
			}
		}

		if (selectCount == 0)
		{
			if (selectAudioCount == 1)
			{
				m_pScene->GetGame()->GetAudioSystem()->PlaySE(L"sound/select.wav", 0.1f);
			}
			selectAudioCount = 0;

			m_pSelectedSpriteComp->SetActive(true);
			m_pSelectedSpriteComp1->SetActive(false);
			m_pSelectedSpriteComp2->SetActive(false);

			if (kbLog.pressed.Enter)
			{
				m_pScene->GetGame()->GetAudioSystem()->PlaySE(L"sound/click.wav", 0.1f);
				isPause = false;
			}
		}
		else if (selectCount == 1)
		{
			if (selectAudioCount == 0)
			{
				m_pScene->GetGame()->GetAudioSystem()->PlaySE(L"sound/select.wav", 0.1f);
			}
			selectAudioCount = 1;
			m_pSelectedSpriteComp->SetActive(false);
			m_pSelectedSpriteComp1->SetActive(true);
			m_pSelectedSpriteComp2->SetActive(false);

			if (kbLog.pressed.Enter)
			{
				m_pScene->GetGame()->GetAudioSystem()->StopBGM(L"sound/bgm.wav");
				m_pScene->GetGame()->GetAudioSystem()->PlaySE(L"sound/click.wav", 0.1f);
				// ステージ選択シーンをロード
				bool isquit = true;
				GetScene()->SetIsQuit(isquit);
				std::wstring nextSceneName = L"SelectStage";
				GetScene()->SetNextSceneName(nextSceneName);
			}
		}
		else if (selectCount == 2)
		{
			if (selectAudioCount == 1)
			{
				m_pScene->GetGame()->GetAudioSystem()->PlaySE(L"sound/select.wav", 0.1f);
			}
			selectAudioCount = 0;

			m_pSelectedSpriteComp->SetActive(false);
			m_pSelectedSpriteComp1->SetActive(false);
			m_pSelectedSpriteComp2->SetActive(true);

			if (kbLog.pressed.Enter)
			{
				m_pScene->GetGame()->GetAudioSystem()->StopBGM(L"sound/bgm.wav");
				m_pScene->GetGame()->GetAudioSystem()->PlaySE(L"sound/click.wav", 0.1f);
				// タイトルシーンをロード
				bool isquit = true;
				GetScene()->SetIsQuit(isquit);
				std::wstring nextSceneName = L"Title";
				GetScene()->SetNextSceneName(nextSceneName);
			}
		}
	}
	else if (!m_pScene->GetIsPause())
	{
		m_pBackGroundSpriteComp->SetActive(false);
		m_pSelectSpriteComp->SetActive(false);
		m_pSelectedSpriteComp->SetActive(false);
		m_pSelectSpriteComp1->SetActive(false);
		m_pSelectedSpriteComp1->SetActive(false);
		m_pSelectSpriteComp2->SetActive(false);
		m_pSelectedSpriteComp2->SetActive(false);
		m_pLeftClickSpriteComp->SetActive(false);
		m_pMoveSpriteComp->SetActive(false);
		m_pClearTextComp->SetActive(false);
		m_pPauseTextComp->SetActive(false);
		m_pGameOverTextComp->SetActive(false);
		m_pTotalShotTextComp->SetActive(false);
		m_pRestartTextComp->SetActive(false);
		m_pResumeTextComp->SetActive(false);
		m_pTitleTextComp->SetActive(false);
		m_pStageSelectComp->SetActive(false);
		m_pFontMoveTextComp->SetActive(false);
		m_pFontFireTextComp->SetActive(false);
		limitCount = 0;
	}

	if (m_pScene->GetIsClear())
	{
		m_pScene->GetGame()->GetAudioSystem()->StopBGM(L"sound/bgm.wav");
		++limitCount;
		if(limitCount == 1)
			showTotalTime = totalTime;
		m_pBackGroundSpriteComp->SetActive(true);
		m_pSelectSpriteComp->SetActive(true);
		m_pSelectedSpriteComp->SetActive(true);
		m_pSelectSpriteComp1->SetActive(true);
		m_pSelectSpriteComp2->SetActive(true);
		m_pClearTextComp->SetActive(true);
		m_pGameOverTextComp->SetActive(false);
		m_pTotalShotTextComp->GetText()->SetTextString(L"TOTAL SHOT : " + std::to_wstring(totalFireBullets));
		m_pTotalShotTextComp->SetActive(true);
		m_pRestartTextComp->SetActive(true);
		m_pTitleTextComp->SetActive(true);
		m_pStageSelectComp->SetActive(true);

		auto kb = GetScene()->GetGame()->GetInputSystem()->GetKeybord()->GetState();
		auto& kbLog = GetScene()->GetGame()->GetInputSystem()->GetKeyLog();

		if (kb.Up)
		{
			if (selectDelayTime > 0.1f)
			{
				--selectCount;
				if (selectCount <= 0)
					selectCount = 0;
				selectDelayTime = 0.0f;
			}
		}
		else if (kb.Down)
		{
			if (selectDelayTime > 0.1f)
			{
				++selectCount;
				if (selectCount >= 2)
					selectCount = 2;
				selectDelayTime = 0.0f;
			}
		}

		if (selectCount == 0)
		{
			if (selectAudioCount == 1)
			{
				m_pScene->GetGame()->GetAudioSystem()->PlaySE(L"sound/select.wav", 0.1f);
			}
			selectAudioCount = 0;
			m_pSelectedSpriteComp->SetActive(true);
			m_pSelectedSpriteComp1->SetActive(false);
			m_pSelectedSpriteComp2->SetActive(false);

			if (kbLog.pressed.Enter)
			{
				m_pScene->GetGame()->GetAudioSystem()->PlaySE(L"sound/click.wav", 0.1f);
				// ステージシーンをロード
				bool isquit = true;
				GetScene()->SetIsQuit(isquit);
				std::wstring nextSceneName = L"";
				if (m_pScene->GetName() == L"Stage")
					nextSceneName = L"Stage";
				else if (m_pScene->GetName() == L"Stage2")
					nextSceneName = L"Stage2";
				else if (m_pScene->GetName() == L"Stage3")
					nextSceneName = L"Stage3";
				GetScene()->SetNextSceneName(nextSceneName);
			}
		}
		else if (selectCount == 1)
		{
			if (selectAudioCount == 0)
			{
				m_pScene->GetGame()->GetAudioSystem()->PlaySE(L"sound/select.wav", 0.1f);
			}
			selectAudioCount = 1;
			m_pSelectedSpriteComp->SetActive(false);
			m_pSelectedSpriteComp1->SetActive(true);
			m_pSelectedSpriteComp2->SetActive(false);

			if (kbLog.pressed.Enter)
			{
				m_pScene->GetGame()->GetAudioSystem()->PlaySE(L"sound/click.wav", 0.1f);
				// ステージ選択シーンをロード
				bool isquit = true;
				GetScene()->SetIsQuit(isquit);
				std::wstring nextSceneName = L"SelectStage";
				GetScene()->SetNextSceneName(nextSceneName);
			}
		}
		else if (selectCount == 2)
		{
			if (selectAudioCount == 1)
			{
				m_pScene->GetGame()->GetAudioSystem()->PlaySE(L"sound/select.wav", 0.1f);
			}
			selectAudioCount = 0;
			m_pSelectedSpriteComp->SetActive(false);
			m_pSelectedSpriteComp1->SetActive(false);
			m_pSelectedSpriteComp2->SetActive(true);

			if (kbLog.pressed.Enter)
			{
				m_pScene->GetGame()->GetAudioSystem()->PlaySE(L"sound/click.wav", 0.1f);
				// タイトルシーンをロード
				bool isquit = true;
				GetScene()->SetIsQuit(isquit);
				std::wstring nextSceneName = L"Title";
				GetScene()->SetNextSceneName(nextSceneName);
			}
		}
	}

	if (m_pScene->GetIsGameOver())
	{
		m_pScene->GetGame()->GetAudioSystem()->StopBGM(L"sound/bgm.wav");
		++limitCount;
		if(limitCount == 1)
			showTotalTime = totalTime;
		m_pBackGroundSpriteComp->SetActive(true);
		m_pSelectSpriteComp->SetActive(true);
		m_pSelectedSpriteComp->SetActive(true);
		m_pSelectSpriteComp1->SetActive(true);
		m_pSelectSpriteComp2->SetActive(true);
		m_pClearTextComp->SetActive(false);
		m_pGameOverTextComp->SetActive(true);
		m_pTotalShotTextComp->GetText()->SetTextString(L"TOTAL SHOT : " + std::to_wstring(totalFireBullets));
		m_pTotalShotTextComp->SetActive(true);
		m_pRestartTextComp->SetActive(true);
		m_pTitleTextComp->SetActive(true);
		m_pStageSelectComp->SetActive(true);

		auto kb = GetScene()->GetGame()->GetInputSystem()->GetKeybord()->GetState();
		auto& kbLog = GetScene()->GetGame()->GetInputSystem()->GetKeyLog();

		if (kb.Up)
		{
			if (selectDelayTime > 0.1f)
			{
				--selectCount;
				if (selectCount <= 0)
					selectCount = 0;
				selectDelayTime = 0.0f;
			}
		}
		else if (kb.Down)
		{
			if (selectDelayTime > 0.1f)
			{
				++selectCount;
				if (selectCount >= 2)
					selectCount = 2;
				selectDelayTime = 0.0f;
			}
		}

		if (selectCount == 0)
		{
			if (selectAudioCount == 1)
			{
				m_pScene->GetGame()->GetAudioSystem()->PlaySE(L"sound/select.wav", 0.1f);
			}
			selectAudioCount = 0;
			m_pSelectedSpriteComp->SetActive(true);
			m_pSelectedSpriteComp1->SetActive(false);
			m_pSelectedSpriteComp2->SetActive(false);

			if (kbLog.pressed.Enter)
			{
				m_pScene->GetGame()->GetAudioSystem()->PlaySE(L"sound/click.wav", 0.1f);
				// ステージシーンをロード
				bool isquit = true;
				GetScene()->SetIsQuit(isquit);
				std::wstring nextSceneName = L"";
				if (m_pScene->GetName() == L"Stage")
					nextSceneName = L"Stage";
				else if (m_pScene->GetName() == L"Stage2")
					nextSceneName = L"Stage2";
				else if (m_pScene->GetName() == L"Stage3")
					nextSceneName = L"Stage3";
				GetScene()->SetNextSceneName(nextSceneName);
			}
		}
		else if (selectCount == 1)
		{
			if (selectAudioCount == 0)
			{
				m_pScene->GetGame()->GetAudioSystem()->PlaySE(L"sound/select.wav", 0.1f);
			}
			selectAudioCount = 1;
			m_pSelectedSpriteComp->SetActive(false);
			m_pSelectedSpriteComp1->SetActive(true);
			m_pSelectedSpriteComp2->SetActive(false);

			if (kbLog.pressed.Enter)
			{
				m_pScene->GetGame()->GetAudioSystem()->PlaySE(L"sound/click.wav", 0.1f);
				// ステージ選択シーンをロード
				bool isquit = true;
				GetScene()->SetIsQuit(isquit);
				std::wstring nextSceneName = L"SelectStage";
				GetScene()->SetNextSceneName(nextSceneName);
			}
		}
		else if (selectCount == 2)
		{
			if (selectAudioCount == 1)
			{
				m_pScene->GetGame()->GetAudioSystem()->PlaySE(L"sound/select.wav", 0.1f);
			}
			selectAudioCount = 0;
			m_pSelectedSpriteComp->SetActive(false);
			m_pSelectedSpriteComp1->SetActive(false);
			m_pSelectedSpriteComp2->SetActive(true);

			if (kbLog.pressed.Enter)
			{
				m_pScene->GetGame()->GetAudioSystem()->PlaySE(L"sound/click.wav", 0.1f);
				// タイトルシーンをロード
				bool isquit = true;
				GetScene()->SetIsQuit(isquit);
				std::wstring nextSceneName = L"Title";
				GetScene()->SetNextSceneName(nextSceneName);
			}
		}
	}
}

void ResultScreen::UpdatePosition(int width, int height)
{
	m_pBackGroundSpriteComp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2, height / 2));
	m_pSelectSpriteComp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2, height / 2 + 100.0f));
	m_pSelectedSpriteComp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2, height / 2 + 100.0f));
	m_pSelectSpriteComp1->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2, height / 2 + 200.0f));
	m_pSelectedSpriteComp1->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2, height / 2 + 200.0f));
	m_pSelectSpriteComp2->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2, height / 2 + 300.0f));
	m_pSelectedSpriteComp2->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(width / 2, height / 2 + 300.0f));
	m_pClearTextComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, height / 2 - 250.0f));
	m_pGameOverTextComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, height / 2 - 250.0f));
	m_pTotalShotTextComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, height / 2 - 50.0f));
	m_pRestartTextComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, height / 2 + 100.0f));
	m_pStageSelectComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, height / 2 + 200.0f));
	m_pTitleTextComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, height / 2 + 300.0f));
	m_pPauseTextComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, height / 2 - 150.0f));
	m_pFontMoveTextComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(100.0f, height - 25.0f));
	m_pFontFireTextComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(250.0f, height - 25.0f));
	m_pMoveSpriteComp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(100.0f, height - 100.0f));
	m_pLeftClickSpriteComp->GetSprite()->SetSpritePosition(DirectX::XMFLOAT2(250.0f, height - 100.0f));
	m_pResumeTextComp->GetText()->SetTextPosition(DirectX::XMFLOAT2(width / 2, height / 2 + 100.0f));
}
