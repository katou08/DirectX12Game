#pragma once
#include "pch.h"
#include "Game/Scene.h"

class Camera;
class Player;
class Entity;
class SpriteComponent;
class TextComponent;
class MeshComponent;
class Title : public Scene
{
public:
	Title(Game* game, const std::wstring& sceneName = L"");
	virtual ~Title();
	virtual bool LoadData() override;
	void GenerateCubes();
	virtual void Update(float deltaTime) override;

private:
	Camera* m_pCamera = nullptr;
	Entity* m_pSpriteStart = nullptr;
	Entity* m_pSpriteQuit = nullptr;
	Entity* m_pFontStart = nullptr;
	Entity* m_pFontQuit = nullptr;
	Entity* m_pSpriteTitle = nullptr;
	Entity* m_pCubes = nullptr;
	MeshComponent* m_pCubesMesh = nullptr;
	std::vector<Vector3> m_Axis = { {0.0f, 1.0f, 0.0f} };

	SpriteComponent* m_pSpriteStartComp = nullptr;
	SpriteComponent* m_pSpriteQuitComp = nullptr;
	TextComponent* m_pFontStartComp = nullptr;
	TextComponent* m_pFontQuitComp = nullptr;

	int width = 1;
	int height = 1;

	int selectCount = 0;
	int selectAudioCount = 0;
	float loadTime = 0.0f;
	float m_addDeltaTime = 0.0f;
};