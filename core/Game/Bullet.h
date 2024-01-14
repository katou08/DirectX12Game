#pragma once
#include "pch.h"
#include "Entity/Entity.h"

class MeshComponent;
class CollisionTypeComponent;

class Bullet : public Entity
{
	enum BulletType
	{
		NormalBullet = 0,
		CircularBullet = 1,
		SpiralBullet = 2
	};
public:
	Bullet();
	virtual ~Bullet();
	void SetMoveSpeed(const float& speed) { m_moveValue = speed; }
	void SetActiveBulletIndex(const uint32_t& index) { m_isActiveBullet[index] = true; }
	void SetOtherBoundBox(const DirectX::BoundingBox& otherbox) { m_OtherBoundingBox = otherbox; }
	void SetDeActiveBulletIndex(const uint32_t& index);
	void SetBulletType(const int& type) { m_eBulletType = BulletType(type); }
	void SetInstanceVector();
protected:
	virtual void OnCreate();
	virtual void OnUpdate(float deltaTime);

private:
	const Vector3& RotateVectorAroundY(const Vector3& originalVector, float angleInRadians);

	Entity* m_pEntity = nullptr;
	MeshComponent* m_pMeshComponent = nullptr;
	CollisionTypeComponent* m_pBulletCollisionTypeComponent = nullptr;
	float m_moveValue = 15.0f;
	std::vector<float> m_survTimes = { 0.0f };
	std::vector<bool> m_isActiveBullet = { false };
	std::vector<Vector3> m_FirstBulletFireDir = { Vector3(0.0f, 0.0f, 0.0f) };
	DirectX::BoundingBox m_OtherBoundingBox = DirectX::BoundingBox();
	BulletType m_eBulletType = BulletType::NormalBullet;
};