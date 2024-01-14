#pragma once
#include "pch.h"
#include "Game/Scene.h"

class Camera;
class Entity;
class Bullet;
class Player;
class ResultScreen;
class TextComponent;
class SpriteComponent;
class MeshComponent;
class Enemy;

class Stage : public Scene
{
public:
	enum StageState
	{
		Playing = 0,
		GameOver,
		GameCompleted,
		Pause,
		Start,
	};

	Stage(Game* game, const std::wstring& sceneName = L"");
	virtual ~Stage();
	virtual bool LoadData() override;
	virtual void Update(float deltaTime) override;

	void DeleteEnemy();

private:
	void AddChaseEnemy();
	void GenerateCubes();
	void UpdateCubes(float deltaTime);
	void UpdateSpriteAndTextPosition();

	Camera* m_pCamera = nullptr;
	Player* m_pPlayer = nullptr;
	Entity* m_pMap = nullptr;
	Entity* m_pLimitTimeText = nullptr;
	TextComponent* m_pLimitTimeTextComp = nullptr;

	Enemy* m_pCoreEnemy = nullptr;
	Enemy* m_pCircularEnemy = nullptr;
	Enemy* m_pCircularEnemy1 = nullptr;
	Enemy* m_ChaseEnemy = nullptr;
	Enemy* m_ChaseEnemy1 = nullptr;

	Entity* m_pStartTitleText = nullptr;
	TextComponent* m_pStartTitleTextComp = nullptr;

	Entity* m_pPauseSprite = nullptr;
	Entity* m_pPauseText = nullptr;
	SpriteComponent* m_pPauseSpriteComp = nullptr;
	TextComponent* m_pPauseTextComp = nullptr;

	int CoreShowCount = 0;
	int ChaseShowCount = 0;
	int CircularShowCount = 0;
	float m_StartTime = 0.0f;

	ResultScreen* m_pResultScreen = nullptr;

	Entity* m_pCubes = nullptr;
	MeshComponent* m_pCubesMesh = nullptr;
	std::vector<Vector3> m_Axis = { {0.0f, 1.0f, 0.0f} };
	float m_addDeltaTime = 0.0f;

	float m_LimitTime = 120.0f;
	StageState m_State = StageState::Start;
};