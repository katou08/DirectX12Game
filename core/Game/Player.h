#pragma once
#include "pch.h"
#include "Entity/Entity.h"

class MeshComponent;
class CollisionTypeComponent;
class Bullet;

class Player : public Entity
{
public:
	Player();
	virtual ~Player();
	void SetOtherBoundBox(const DirectX::BoundingBox& otherbox) { m_OtherBoundingBox = otherbox; }
	void TakeDamage();
	const Vector3& GetOldPos() const { return m_OldPos; }
	Bullet* GetBulletEntity() const { return m_pBullets; }
	const float& GetHealth() const { return m_HealthCount; }
	const int& GetTotalFireBulletCount() const { return m_TotalFireBulletCount; }
protected:
	virtual void OnCreate();
	virtual void OnUpdate(float deltaTime);

private:
	Entity* m_pEntity = nullptr;
	Bullet* m_pBullets = nullptr;
	MeshComponent* m_pPlayerMeshComponent = nullptr;
	MeshComponent* m_pBulletMeshComponent = nullptr;
	CollisionTypeComponent* m_pPlayerCollisionTypeComponent = nullptr;

	float m_moveValue = 8.0f;
	float m_forward = 0.0f;
	float m_rightward = 0.0f;
	Vector3 m_worldUpward = Vector3(0.0f, 1.0f, 0.0f);
	Vector3 m_worldRightDir = Vector3(1.0f, 0.0f, 0.0f);
	Vector3 m_worldForwardDir = Vector3(0.0f, 0.0f, 1.0f);

	DirectX::BoundingBox m_OtherBoundingBox = DirectX::BoundingBox();
	int m_HealthCount = 3;
	float m_DelayDamegeTime = 0.0f;
	float m_DelayFiring = 0.0f;
	int m_BulletCount = 0;
	int m_TotalFireBulletCount = 0;
	Vector3 m_OldPos = Vector3(0.0f, 0.0f, 0.0f);
};