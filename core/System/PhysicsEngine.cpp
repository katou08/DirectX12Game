#include "System/PhysicsEngine.h"
#include "Entity/MeshComponent.h"
#include "Entity/TransformComponent.h"
#include "Entity/CollisionTypeComponent.h"
#include "Resource/Mesh.h"
#include "Game/Enemy.h"
#include "Game/Player.h"
#include "Game/Bullet.h"
#include "System/ImguiSystem.h";
#include "Graphics/GraphicsEngine.h"
#include "Game/Game.h"

PhysicsEngine::PhysicsEngine(Game* game) :m_pGame(game)
{
}

PhysicsEngine::~PhysicsEngine()
{
}

bool PhysicsEngine::Initialize()
{
	return true;
}

void PhysicsEngine::Update()
{
	
	for (auto collision : m_pCollisionTypeComp)
	{
		for (auto collision1 : m_pCollisionTypeComp)
		{
			if (collision == collision1) continue;
			auto it = m_pCheckedCollisionTypeComp.find(collision1);
			if (it != m_pCheckedCollisionTypeComp.end()) continue;

			if (collision->GetCollisionType() == CollisionType::PlayerCollision)
			{
				if (collision1->GetCollisionType() == CollisionType::WallCollision)
					ProcessPlayerAndWallCollision(collision, collision1);
				else if (collision1->GetCollisionType() == CollisionType::EnemyCollision)
					ProcessPlayerAndEnemyCollision(collision, collision1);
			}
			else if (collision->GetCollisionType() == CollisionType::WallCollision)
			{
				if(collision1->GetCollisionType() == CollisionType::PlayerCollision)
					ProcessPlayerAndWallCollision(collision1, collision);
				else if (collision1->GetCollisionType() == CollisionType::BulletCollision)
					ProcessBulletAndWallCollision(collision1, collision);
			}
			else if (collision->GetCollisionType() == CollisionType::BulletCollision)
			{
				if (collision1->GetCollisionType() == CollisionType::WallCollision)
					ProcessBulletAndWallCollision(collision, collision1);
			}
			else if (collision->GetCollisionType() == CollisionType::EnemyCollision)
			{
				if (collision1->GetCollisionType() == CollisionType::PlayerCollision)
					ProcessPlayerAndEnemyCollision(collision1, collision);
			}
		}
		m_pCheckedCollisionTypeComp.emplace(collision);
	}
	m_pCheckedCollisionTypeComp.clear();
}

void PhysicsEngine::AddComponent(Component* comp)
{
	if (auto collisionComp = dynamic_cast<CollisionTypeComponent*>(comp))
		m_pCollisionTypeComp.emplace(collisionComp);
}

void PhysicsEngine::RemoveComponent(Component* comp)
{
	if (auto collisionComp = dynamic_cast<CollisionTypeComponent*>(comp))
		m_pCollisionTypeComp.erase(collisionComp);
}

void PhysicsEngine::ProcessPlayerAndWallCollision(CollisionTypeComponent* player, CollisionTypeComponent* other)
{
	auto playerEntity = dynamic_cast<Player*>(player->GetEntity());
	auto playerMesh = playerEntity->GetComponent<MeshComponent>()->GetMesh();
	auto otherEntity = other->GetEntity();
	auto otherMesh = otherEntity->GetComponent<MeshComponent>()->GetMesh();
	auto oldPos = playerEntity->GetOldPos();

	if (playerMesh->GetBoundBox(0).Intersects(otherMesh->GetBoundBox(0)))
	{
		playerEntity->GetTransform()->SetPosition(oldPos);
		playerMesh->SetPosition(playerEntity->GetTransform()->GetPosition(), 0);
	}
}

void PhysicsEngine::ProcessPlayerAndEnemyCollision(CollisionTypeComponent* player, CollisionTypeComponent* other)
{
	auto playerEntity = dynamic_cast<Player*>(player->GetEntity());
	auto EnemyEntity = dynamic_cast<Enemy*>(other->GetEntity());
	auto EnemybulletEntity = dynamic_cast<Enemy*>(other->GetEntity())->GetBulletEntity();
	auto PlayerbulletEntity = playerEntity->GetBulletEntity();
	auto oldPos = playerEntity->GetOldPos();
	auto playerBulletCount = PlayerbulletEntity->GetComponent<MeshComponent>()->GetMesh()->GetInstanceCount();
	auto enemyBulletCount = EnemybulletEntity->GetComponent<MeshComponent>()->GetMesh()->GetInstanceCount();
	auto playerMesh = playerEntity->GetComponent<MeshComponent>()->GetMesh();
	auto playerBulletMesh = PlayerbulletEntity->GetComponent<MeshComponent>()->GetMesh();
	auto enemyMesh = EnemyEntity->GetComponent<MeshComponent>()->GetMesh();
	auto enemyBulletMesh = EnemybulletEntity->GetComponent<MeshComponent>()->GetMesh();

	// プレイヤーとエネミーの衝突判定
	if (playerMesh->GetBoundSphere(0).Intersects(enemyMesh->GetBoundSphere(0)))
	{
		playerEntity->GetTransform()->SetPosition(oldPos);
		playerMesh->SetPosition(playerEntity->GetTransform()->GetPosition(), 0);
	}

	for (int i = 0; i < enemyBulletCount; ++i)
	{
		for (int j = 0; j < playerBulletCount; ++j)
		{
			// プレイヤーとエネミーの弾の衝突判定
			if (playerMesh->GetBoundSphere(0).Intersects(enemyBulletMesh->GetBoundSphere(i)))
			{
				EnemybulletEntity->SetDeActiveBulletIndex(i);
				playerEntity->TakeDamage();
			}
			// エネミーの弾とプレイヤーの弾の衝突判定
			else if (enemyBulletMesh->GetBoundSphere(i).Intersects(playerBulletMesh->GetBoundBox(j)))
			{
				EnemybulletEntity->SetDeActiveBulletIndex(i);
				PlayerbulletEntity->SetDeActiveBulletIndex(j);
			}
			// エネミーとプレイヤーの弾の衝突判定
			else if (enemyMesh->GetBoundSphere(0).Intersects(playerBulletMesh->GetBoundSphere(j)))
			{
				PlayerbulletEntity->SetDeActiveBulletIndex(j);
				EnemyEntity->TakeDamage();
			}
		}
	}
}

void PhysicsEngine::ProcessBulletAndWallCollision(CollisionTypeComponent* bullet, CollisionTypeComponent* other)
{
	auto bulletEntity = dynamic_cast<Bullet*>(bullet->GetEntity());
	auto bulletMesh = bulletEntity->GetComponent<MeshComponent>()->GetMesh();
	auto bulletCount = bulletMesh->GetInstanceCount();
	auto otherEntity = other->GetEntity();
	auto otherMesh = otherEntity->GetComponent<MeshComponent>()->GetMesh();

	for (int i = 0; i < bulletCount; ++i)
	{
		if (bulletMesh->GetBoundBox(i).Intersects(otherMesh->GetBoundBox(0)))
			bulletEntity->SetDeActiveBulletIndex(i);
	}
}
