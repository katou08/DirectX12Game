#pragma once
#include "pch.h"
#include "Entity/Entity.h"

class MeshComponent;
class Bullet;
class CollisionTypeComponent;

class Enemy : public Entity
{
public:
	enum EnemyType
	{
		ChaseEnemy = 0,
		CircularEnemy = 1,
		SpiralEnemy = 2,
		CoreEnemy = 3
	};

	Enemy();
	virtual ~Enemy();
	void SetPlayerPos(const Vector3& playerPos) { m_PlayerPos = playerPos; }
	void SetEnemyType(const int type) { m_eEnemyType = EnemyType(type); }
	void LoadData();
	const int& GetEnemyType() const { return m_eEnemyType; }
	Bullet* GetBulletEntity() const { return m_pBullets; }
	const bool& GetDamaged() const { return m_isDamaged; }
	void TakeDamaged() { m_isDamaged = true; }
	void TakeDamage();
	void SetHealth(const int& value) { m_HealthCount = value; }
	void SetIsMove(const bool& value) { m_isMove = value; }
	const int& GetHealth() const { return m_HealthCount; }
	void RemoveBulletEntity();
protected:
	virtual void OnCreate();
	virtual void OnUpdate(float deltaTime);

private:
	Bullet* m_pBullets = nullptr;
	MeshComponent* m_pMeshComponent = nullptr;
	MeshComponent* m_pBulletMeshComponent = nullptr;
	CollisionTypeComponent* m_pEnemyCollisionTypeComponent = nullptr;

	float m_forward = 0.0f;
	float m_rightward = 0.0f;
	float m_moveValue = 3.0f;
	float m_DelayFiringBulletTime = 0.0f;
	int m_BulletCount = 0;
	int m_HealthCount = 1;
	float m_DelayDamegeTime = 0.5f;
	Vector3 m_worldUpward = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 m_worldRightDir = Vector3(1.0f, 0.0f, 0.0f);
	Vector3 m_worldForwardDir = Vector3(0.0f, 0.0f, 1.0f);
	Vector3 m_PlayerPos = Vector3(0.0f, 0.0f, 0.0f);
	EnemyType m_eEnemyType = EnemyType::ChaseEnemy;
	bool m_isDamaged = false;
	bool m_isMove = false;
};