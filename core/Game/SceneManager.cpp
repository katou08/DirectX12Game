#include "Game/SceneManager.h"
#include "Game/Game.h"
#include "Game/Scene.h"
#include "Game/Player.h"
#include "Entity/Entity.h"
#include "Game/Title.h"
#include "Game/Stage.h"
#include "Game/Stage2.h"
#include "Game/Stage3.h"
#include "Game/SelectStage.h"
#include "Game/LoadingScene.h"

SceneManager::SceneManager(Game* game) : m_pGame(game)
{
	m_pLoadingScene = std::make_unique<LoadingScene>(m_pGame, L"Loading");
	m_pLoadingScene->LoadData();
}

SceneManager::~SceneManager()
{
	if (m_pNowScene)
	{
		delete m_pNowScene;
		m_pNowScene = nullptr;
	}
}

bool SceneManager::LoadScene(const std::wstring& sceneName)
{
	if (m_pNowScene)
	{
		delete m_pNowScene;
		m_pNowScene = nullptr;
	}

	// シーンの作成
	if (sceneName == L"Title")
		m_pNowScene = new Title(m_pGame, L"Title");
	else if (sceneName == L"SelectStage")
		m_pNowScene = new SelectStage(m_pGame, L"SelectStage");
	else if (sceneName == L"Stage")
		m_pNowScene = new Stage(m_pGame, L"Stage");
	else if (sceneName == L"Stage2")
		m_pNowScene = new Stage2(m_pGame, L"Stage2");
	else if (sceneName == L"Stage3")
		m_pNowScene = new Stage3(m_pGame, L"Stage3");

	// TODO ロードが完了するまで待機処理
	if (m_pNowScene)
	{
		m_pNowScene->LoadData();
	}

	return true;
}

void SceneManager::Update(float deltaTime)
{
	if (m_pNowScene)
	{
		m_pNowScene->Update(deltaTime);

		IsQuit = m_pNowScene->GetIsQuit();
		if (IsQuit)
		{
			std::wstring nextSceneName = m_pNowScene->GetNextSceneName();
			if (nextSceneName == L"")
			{
				IsQuit = false;
				m_pGame->m_isRunning = false;
			}
			else
			{
				IsQuit = false;
				ChangeScene(nextSceneName);
			}
		}
	}
}

void SceneManager::ChangeScene(const std::wstring& nextName)
{
	LoadScene(nextName);
}
