#include "Game/Game.h"
#include "Window.h"
#include "Game/Display.h"
#include "Graphics/GraphicsEngine.h"
#include "System/PhysicsEngine.h"
#include "System/InputSystem.h"
#include "System/AudioSystem.h"
#include "Game/Camera.h"
#include "Game/Light.h"
#include "Game/Scene.h"
#include "Game/SceneManager.h"
#include "pch.h"
#include <Windows.h>

Game::Game()
{
}

Game::~Game()
{
	CoUninitialize(); //!< COM解放
}

bool Game::Initialize()
{	
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) //!< COM初期化
	{
		ThrowFailed("COM初期化に失敗しました.");
		return false;
	}

	// ディスプレイの初期化(レンダーシステムを初期化した後に)
	m_pDisplay = std::make_unique<Display>(this);
	if (!m_pDisplay->Initialize())
		ThrowFailed("ディスプレイの初期化に失敗しました");

	// グラフィックスエンジンの初期化
	m_pGraphicsEngine = std::make_unique<GraphicsEngine>(this);
	if(!m_pGraphicsEngine->Initialize())
		ThrowFailed("グラフィックスエンジンの初期化に失敗しました");

	// 物理エンジン(衝突判定用)の初期化
	m_pPhysicsEngine = std::make_unique<PhysicsEngine>(this);
	if(!m_pPhysicsEngine->Initialize())
		ThrowFailed("物理エンジンの初期化に失敗しました");

	// オーディオシステムの初期化
	m_pAudioSystem = std::make_unique<AudioSystem>(this);
	if(!m_pAudioSystem->Initialize())
		ThrowFailed("オーディオシステムの初期化に失敗しました");

	// インプットシステムの初期化
	m_pInputSystem = std::make_unique<InputSystem>(this);
	if(!m_pInputSystem->Initialize())
		ThrowFailed("インプットシステムの初期化に失敗しました.");

	LoadData();

	return true;
}

void Game::RunLoop()
{
	while (m_isRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown()
{
	UnloadData();

}

void Game::ProcessInput()
{
	MSG msg = {};
	// メッセージループ
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			m_isRunning = false;
			continue;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

void Game::UpdateGame()
{
	ComputeDeltaTime(); //!< デルタタイムの計算
	m_pInputSystem->Update();
	m_pAudioSystem->Update(); //!< オーディオシステムの更新
	m_pSceneManager->Update(deltaTime); //!< シーンの更新
	m_pPhysicsEngine->Update(); //!< 物理エンジンの更新
}

void Game::GenerateOutput()
{
	m_pGraphicsEngine->Update();
}

void Game::LoadData()
{
	// カメラの初期化
	m_pCamera = std::make_unique<Camera>(this);

	// ディレクショナルライトの初期化
	m_pLight = std::make_unique<Light>(this);

	// サウンドの初期化
	m_pAudioSystem->SetAudioFile(L"sound/beam.wav");
	m_pAudioSystem->SetAudioFile(L"sound/destroy.wav");
	m_pAudioSystem->SetAudioFile(L"sound/select.wav");
	m_pAudioSystem->SetAudioFile(L"sound/click.wav");
	m_pAudioSystem->SetAudioFile(L"sound/playerDamage.wav");
	m_pAudioSystem->SetAudioFile(L"sound/bgm.wav");
	m_pAudioSystem->SetAudioFile(L"sound/title.wav");
	m_pAudioSystem->SetAudioFile(L"sound/selectStage.wav");

	// シーンマネージャーの初期化
	m_pSceneManager = std::make_unique<SceneManager>(this);
	m_pSceneManager->LoadScene(L"Title");
}

void Game::UnloadData()
{
}

void Game::ComputeDeltaTime()
{
	auto currentTime = std::chrono::system_clock::now(); //!< 現在の時間を取得
	auto elapsedSeconds = std::chrono::duration<double>(); //!< 初期値は0
	if (m_previousTime.time_since_epoch().count())
		elapsedSeconds = currentTime - m_previousTime;
	m_previousTime = currentTime;
	deltaTime = static_cast<float>(elapsedSeconds.count());
	m_totalTime += deltaTime;
}
