#pragma once
#include "pch.h"
#include "Game/Scene.h"

class Camera;
class Entity;
class SpriteComponent;
class TextComponent;
class MeshComponent;

class SelectStage : public Scene
{
public:
	SelectStage(Game* game, const std::wstring& sceneName = L"");
	virtual ~SelectStage();
	virtual bool LoadData() override;
	void GenerateCubes();
	virtual void Update(float deltaTime) override;

	void UpdateCubes(float deltaTime);

	void UpdatePosition();

private:
	Camera* m_pCamera = nullptr;
	Entity* m_pSpriteStage1 = nullptr;
	Entity* m_pSpriteStage2 = nullptr;
	Entity* m_pSpriteStage3 = nullptr;
	Entity* m_pSpriteSelectedStage1 = nullptr;
	Entity* m_pSpriteSelectedStage2 = nullptr;
	Entity* m_pSpriteSelectedStage3 = nullptr;
	Entity* m_pSpriteSelectArrow = nullptr;
	Entity* m_pSpriteEnterArrow = nullptr;
	Entity* m_pSpriteTitle = nullptr;
	Entity* m_pSpriteSelectedTitle = nullptr;
	Entity* m_pFontSelectStage = nullptr;
	Entity* m_pFontTitle = nullptr;
	Entity* m_pFontSelect = nullptr;
	Entity* m_pFontDecision = nullptr;
	Entity* m_pFontStage1 = nullptr;
	Entity* m_pFontStage2 = nullptr;
	Entity* m_pFontStage3 = nullptr;
	Entity* m_pCubes = nullptr;

	SpriteComponent* m_pSpriteStage1Comp = nullptr;
	SpriteComponent* m_pSpriteStage2Comp = nullptr;
	SpriteComponent* m_pSpriteStage3Comp = nullptr;
	SpriteComponent* m_pSpriteSelectedStage1Comp = nullptr;
	SpriteComponent* m_pSpriteSelectedStage2Comp = nullptr;
	SpriteComponent* m_pSpriteSelectedStage3Comp = nullptr;
	SpriteComponent* m_pSpriteSelectArrowComp = nullptr;
	SpriteComponent* m_pSpriteEnterComp = nullptr;
	SpriteComponent* m_pSpriteLeftArrowComp = nullptr;
	SpriteComponent* m_pSpriteRightArrowComp = nullptr;
	SpriteComponent* m_pSpriteTitleComp = nullptr;
	SpriteComponent* m_pSpriteSelectedTitleComp = nullptr;
	TextComponent* m_pFontSelectStageComp = nullptr;
	TextComponent* m_pFontTitleComp = nullptr;
	TextComponent* m_pFontSelectComp = nullptr;
	TextComponent* m_pFontDecisionComp = nullptr;
	TextComponent* m_pFontStage1Comp = nullptr;
	TextComponent* m_pFontStage2Comp = nullptr;
	TextComponent* m_pFontStage3Comp = nullptr;
	MeshComponent* m_pCubesMesh = nullptr;
	std::vector<Vector3> m_Axis = { {0.0f, 1.0f, 0.0f} };

	int width = 1;
	int height = 1;

	int selectCount = 0;
	int selectTitleCount = 0;
	int selectAudioCount = 0;
	int selectTitleAudioCount = 0;
	float selectDelayTime = 0.1f;
	float loadTime = 0.0f;
	float m_addDeltaTime = 0.0f;
};