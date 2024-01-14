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
	CoUninitialize(); //!< COM���
}

bool Game::Initialize()
{	
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED))) //!< COM������
	{
		ThrowFailed("COM�������Ɏ��s���܂���.");
		return false;
	}

	// �f�B�X�v���C�̏�����(�����_�[�V�X�e�����������������)
	m_pDisplay = std::make_unique<Display>(this);
	if (!m_pDisplay->Initialize())
		ThrowFailed("�f�B�X�v���C�̏������Ɏ��s���܂���");

	// �O���t�B�b�N�X�G���W���̏�����
	m_pGraphicsEngine = std::make_unique<GraphicsEngine>(this);
	if(!m_pGraphicsEngine->Initialize())
		ThrowFailed("�O���t�B�b�N�X�G���W���̏������Ɏ��s���܂���");

	// �����G���W��(�Փ˔���p)�̏�����
	m_pPhysicsEngine = std::make_unique<PhysicsEngine>(this);
	if(!m_pPhysicsEngine->Initialize())
		ThrowFailed("�����G���W���̏������Ɏ��s���܂���");

	// �I�[�f�B�I�V�X�e���̏�����
	m_pAudioSystem = std::make_unique<AudioSystem>(this);
	if(!m_pAudioSystem->Initialize())
		ThrowFailed("�I�[�f�B�I�V�X�e���̏������Ɏ��s���܂���");

	// �C���v�b�g�V�X�e���̏�����
	m_pInputSystem = std::make_unique<InputSystem>(this);
	if(!m_pInputSystem->Initialize())
		ThrowFailed("�C���v�b�g�V�X�e���̏������Ɏ��s���܂���.");

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
	// ���b�Z�[�W���[�v
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
	ComputeDeltaTime(); //!< �f���^�^�C���̌v�Z
	m_pInputSystem->Update();
	m_pAudioSystem->Update(); //!< �I�[�f�B�I�V�X�e���̍X�V
	m_pSceneManager->Update(deltaTime); //!< �V�[���̍X�V
	m_pPhysicsEngine->Update(); //!< �����G���W���̍X�V
}

void Game::GenerateOutput()
{
	m_pGraphicsEngine->Update();
}

void Game::LoadData()
{
	// �J�����̏�����
	m_pCamera = std::make_unique<Camera>(this);

	// �f�B���N�V���i�����C�g�̏�����
	m_pLight = std::make_unique<Light>(this);

	// �T�E���h�̏�����
	m_pAudioSystem->SetAudioFile(L"sound/beam.wav");
	m_pAudioSystem->SetAudioFile(L"sound/destroy.wav");
	m_pAudioSystem->SetAudioFile(L"sound/select.wav");
	m_pAudioSystem->SetAudioFile(L"sound/click.wav");
	m_pAudioSystem->SetAudioFile(L"sound/playerDamage.wav");
	m_pAudioSystem->SetAudioFile(L"sound/bgm.wav");
	m_pAudioSystem->SetAudioFile(L"sound/title.wav");
	m_pAudioSystem->SetAudioFile(L"sound/selectStage.wav");

	// �V�[���}�l�[�W���[�̏�����
	m_pSceneManager = std::make_unique<SceneManager>(this);
	m_pSceneManager->LoadScene(L"Title");
}

void Game::UnloadData()
{
}

void Game::ComputeDeltaTime()
{
	auto currentTime = std::chrono::system_clock::now(); //!< ���݂̎��Ԃ��擾
	auto elapsedSeconds = std::chrono::duration<double>(); //!< �����l��0
	if (m_previousTime.time_since_epoch().count())
		elapsedSeconds = currentTime - m_previousTime;
	m_previousTime = currentTime;
	deltaTime = static_cast<float>(elapsedSeconds.count());
	m_totalTime += deltaTime;
}
