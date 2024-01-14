#pragma once
#include "pch.h"

// 前方宣言
class Display;
class GraphicsEngine;
class PhysicsEngine;
class InputSystem;
class AudioSystem;
class Camera;
class Light;
class Scene;
class Entity;
class SceneManager;

class Game
{
public:
	Game();
	~Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	GraphicsEngine* GetGraphicsEngine() { return m_pGraphicsEngine.get(); }
	PhysicsEngine* GetPhysicsEngine() { return m_pPhysicsEngine.get(); }
	Display* GetDisplay() { return m_pDisplay.get(); }
	Camera* GetCamera() { return m_pCamera.get(); }
	Light* GetLight() { return m_pLight.get(); }
	InputSystem* GetInputSystem() { return m_pInputSystem.get(); }
	AudioSystem* GetAudioSystem() { return m_pAudioSystem.get(); }

private:
	// ゲームループのヘルパー関数
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();
	void ComputeDeltaTime();

	bool m_isRunning = true; //!< ゲームの続行を指示
	std::chrono::system_clock::time_point m_previousTime;
	float m_totalTime = 0.0f;
	float deltaTime = 0.0f;
	float done = 2.0f;

	std::unique_ptr<GraphicsEngine> m_pGraphicsEngine = nullptr;
	std::unique_ptr<PhysicsEngine> m_pPhysicsEngine = nullptr;
	std::unique_ptr<Display> m_pDisplay = nullptr;
	std::unique_ptr<InputSystem> m_pInputSystem = nullptr;
	std::unique_ptr<AudioSystem> m_pAudioSystem = nullptr;
	std::unique_ptr<SceneManager> m_pSceneManager = nullptr;
	std::unique_ptr<Camera> m_pCamera = nullptr;
	std::unique_ptr<Light> m_pLight = nullptr;

private:
	friend class RenderSystem;
	friend class Display;
	friend class SceneManager;
};