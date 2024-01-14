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

class Stage3 : public Scene
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
	Stage3(Game* game, const std::wstring& sceneName = L"");
	virtual ~Stage3();
	virtual bool LoadData() override;
	void AddWalls();
	virtual void Update(float deltaTime) override;

private:
	void DeleteEnemy();
	void AddChaseEnemy();
	void SetPlayerPositionForChaseEnemy();
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
	Enemy* m_pCircularEnemy2 = nullptr;
	Enemy* m_pCircularEnemy3 = nullptr;
	Enemy* m_pCircularEnemy4 = nullptr;
	Enemy* m_pCircularEnemy5 = nullptr;

	Enemy* m_ChaseEnemy = nullptr;
	Enemy* m_ChaseEnemy1 = nullptr;
	Enemy* m_ChaseEnemy2 = nullptr;
	Enemy* m_ChaseEnemy3 = nullptr;
	Enemy* m_ChaseEnemy4 = nullptr;
	Enemy* m_ChaseEnemy5 = nullptr;
	Enemy* m_ChaseEnemy6 = nullptr;
	Enemy* m_ChaseEnemy7 = nullptr;
	Enemy* m_ChaseEnemy8 = nullptr;
	Enemy* m_ChaseEnemy9 = nullptr;
	Enemy* m_ChaseEnemy10 = nullptr;
	Enemy* m_ChaseEnemy11 = nullptr;

	Entity* m_pStartTitleText = nullptr;
	TextComponent* m_pStartTitleTextComp = nullptr;

	Entity* m_pTopRowLeftWall = nullptr;
	Entity* m_pTopRowMiddleWall = nullptr;
	Entity* m_pTopRowRightWall = nullptr;
	Entity* m_pBottomRowLeftWall = nullptr;
	Entity* m_pBottomRowMiddleWall = nullptr;
	Entity* m_pBottomRowRightWall = nullptr;

	Entity* m_pPauseSprite = nullptr;
	Entity* m_pPauseText = nullptr;
	SpriteComponent* m_pPauseSpriteComp = nullptr;
	TextComponent* m_pPauseTextComp = nullptr;

	int CoreShowCount = 0;
	int ChaseShowCount = 0;
	int CircularShowCount = 0;
	int CircularShowLimitCount = 0;
	float m_StartTime = 0.0f;

	ResultScreen* m_pResultScreen = nullptr;

	Entity* m_pCubes = nullptr;
	MeshComponent* m_pCubesMesh = nullptr;
	std::vector<Vector3> m_Axis = { {0.0f, 1.0f, 0.0f} };
	float m_addDeltaTime = 0.0f;

	float m_LimitTime = 120.0f;
	StageState m_State = StageState::Start;
};