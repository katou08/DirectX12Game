#pragma once
#include "pch.h"
#include "Entity/Component.h"

enum CollisionType
{
	Invalied = 0,
	PlayerCollision,
	EnemyCollision,
	BulletCollision,
	WallCollision
};

class CollisionTypeComponent : public Component
{
public:

	CollisionTypeComponent();
	virtual ~CollisionTypeComponent();

	const CollisionType& GetCollisionType() const { return m_CollisionType; }
	void SetCollisionType(const CollisionType& type) { m_CollisionType = type; }


protected:
	virtual void onCreateInternal();

private:
	CollisionType m_CollisionType = CollisionType::Invalied;
};