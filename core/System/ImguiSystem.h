#pragma once
#include "pch.h"
#include "Graphics/ResourceHeap.h"

class Game;
class RenderSystem;
class ImguiSystem
{
public:
	ImguiSystem(Game* game);
	~ImguiSystem();
	bool Initialize();

	void Update();
	void Render();
	void SetText(const Vector3& value) { text = value; }
	void SetText1(const float& value) { text1 = value; }

private:
	Game* m_pGame = nullptr;
	RenderSystem* m_pRenderSystem = nullptr;
	ResourceHeap m_imguiHeap;
	Vector3 text = Vector3(0.0f, 0.0f, 0.0f);
	float text1 = 0.0f;

};