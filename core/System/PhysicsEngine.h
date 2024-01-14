#pragma once
#include "pch.h"

class Game;
class RenderSystem;
class Component;
class CollisionTypeComponent;

class PhysicsEngine
{
public:
	PhysicsEngine(Game* game);
	~PhysicsEngine();
	bool Initialize();
	void Update();

	RenderSystem* GetRenderSystem() { return m_pRenderSystem.get(); }

	void AddComponent(Component* comp);
	void RemoveComponent(Component* comp);

private:
	void ProcessPlayerAndWallCollision(CollisionTypeComponent* player, CollisionTypeComponent* other);
	void ProcessPlayerAndEnemyCollision(CollisionTypeComponent* player, CollisionTypeComponent* other);
	void ProcessBulletAndWallCollision(CollisionTypeComponent* bullet, CollisionTypeComponent* other);
	Game* m_pGame = nullptr;
	std::unique_ptr<RenderSystem> m_pRenderSystem = nullptr;
	std::set<CollisionTypeComponent*> m_pCollisionTypeComp;
	std::set<CollisionTypeComponent*> m_pCheckedCollisionTypeComp;
	int count = 0;
};