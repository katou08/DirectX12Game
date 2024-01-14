#pragma once
#include "pch.h"

class Game;
class RenderSystem;
class Component;
class MeshComponent;
class SpriteComponent;
class TextComponent;
class ImguiSystem;

class GraphicsEngine
{
public:
	GraphicsEngine(Game* game);
	~GraphicsEngine();
	bool Initialize();
	void Update();

	RenderSystem* GetRenderSystem() { return m_pRenderSystem.get(); }
	ImguiSystem* GetImguiSystem() { return m_pImguiSystem.get(); }
	void AddComponent(Component* comp);
	void RemoveComponent(Component* comp);

private:
	Game* m_pGame = nullptr;
	std::unique_ptr<RenderSystem> m_pRenderSystem = nullptr;
	std::unique_ptr<DirectX::GraphicsMemory> m_pGmemory = nullptr;
	std::unique_ptr<ImguiSystem> m_pImguiSystem = nullptr;
	std::set<MeshComponent*> m_pMeshes;
	std::set<SpriteComponent*> m_pSprites;
	std::set<TextComponent*> m_pTexts;
};