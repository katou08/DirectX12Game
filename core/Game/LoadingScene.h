#pragma once
#include "pch.h"
#include "Game/Scene.h"

class Camera;
class Player;
class Entity;
class SpriteComponent;

class LoadingScene : public Scene
{
public:
	LoadingScene(Game* game, const std::wstring& sceneName = L"");
	virtual ~LoadingScene();
	virtual bool LoadData() override;
	bool SetPassive();
	virtual void Update(float deltaTime) override;

private:
	Camera* m_pCamera = nullptr;
	Entity* m_pSprite = nullptr;

	SpriteComponent* m_pSpriteComp = nullptr;

	int width = 1;
	int height = 1;

	float loadTime = 0.0f;
	int loadSpriteCount = 0;
};