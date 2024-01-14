#pragma once
#include "pch.h"

class Game;
class Scene;
class Entity;
class LoadingScene;

class SceneManager
{
public:
	SceneManager(Game* game);
	~SceneManager();
	bool LoadScene(const std::wstring& sceneName);
	void Update(float deltaTime);
	void ChangeScene(const std::wstring& nextName);
	

private:
	Game* m_pGame = nullptr;
	Scene* m_pNowScene = nullptr;
	std::unique_ptr<LoadingScene> m_pLoadingScene = nullptr;
	bool IsQuit = false;

	std::chrono::system_clock::time_point m_previousTime;
	float deltaTime = 0.0f;

	std::future<bool> loadFuture;
};