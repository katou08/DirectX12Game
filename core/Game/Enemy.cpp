#include "Game/Enemy.h"
#include "Game/Bullet.h"
#include "Game/Scene.h"
#include "Game/Game.h"
#include "Entity/Entity.h"
#include "Entity/MeshComponent.h"
#include "Entity/TransformComponent.h"
#include "Entity/CollisionTypeComponent.h"
#include "Resource/Mesh.h"
#include "System/AudioSystem.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::TakeDamage()
{
	if (m_DelayDamegeTime > 0.3f)
	{
		--m_HealthCount;
		m_HealthCount = std::clamp(m_HealthCount, 0, 100);
		m_DelayDamegeTime = 0.0f;
		m_pScene->GetGame()->GetAudioSystem()->PlaySE(L"sound/destroy.wav", 0.1f);
	}
}

void Enemy::RemoveBulletEntity()
{
	if (m_pBullets)
		m_pScene->RemoveEntity(m_pBullets);
}

void Enemy::LoadData()
{
	// 種類ごとにメッシュを生成
	m_pMeshComponent = AddComponent<MeshComponent>();
	if (m_eEnemyType == EnemyType::ChaseEnemy)
	{
		m_pMeshComponent->SetMesh(L"res/chase/chaseEnemy.obj");
		m_pMeshComponent->GetMesh()->SetAllBaseColor(Vector3(0.4f, 0.5f, 0.3f));
	}
	else if (m_eEnemyType == EnemyType::CircularEnemy)
	{
		m_pMeshComponent->SetMesh(L"res/Cylinder/cylinder.obj");
	}
	else if (m_eEnemyType == EnemyType::CoreEnemy)
	{
		m_pMeshComponent->SetMesh(L"res/core/Core.obj");
	}
	// コリジョンを追加
	m_pEnemyCollisionTypeComponent = AddComponent<CollisionTypeComponent>();
	m_pEnemyCollisionTypeComponent->SetCollisionType(CollisionType::EnemyCollision);

	// Bulletを追加
	m_pBullets = m_pScene->AddEntity<Bullet>();
	m_pBullets->GetComponent<TransformComponent>()->SetPosition(GetTransform()->GetPosition());
	m_pBullets->GetComponent<TransformComponent>()->SetRotation(GetTransform()->GetRotation());
	m_pBulletMeshComponent = m_pBullets->GetComponent<MeshComponent>();
	m_pBulletMeshComponent->GetMesh()->SetAllBaseColor(Vector3(0.91f, 0.17, 0.21f));
	m_pBulletMeshComponent->SetActive(true);

	// Bulletタイプごとにインスタンス，BoundingBox，位置，スケールを設定
	if (m_eEnemyType == EnemyType::CircularEnemy)
	{
		m_pBulletMeshComponent->GetMesh()->ChangeInstanceCount(100);
		m_pBullets->SetInstanceVector();
		m_pBullets->SetBulletType(1);
		auto instanceCount = m_pBulletMeshComponent->GetMesh()->GetInstanceCount();
		auto e = m_pBulletMeshComponent->GetMesh()->GetBoundBox(0).Extents;
		for (int i = 0; i < instanceCount; ++i)
		{
			m_pBulletMeshComponent->GetMesh()->SetPosition(GetTransform()->GetPosition(), i);
			m_pBulletMeshComponent->GetMesh()->SetScale(Vector3(1.0f, 1.0f, 1.0f), i);
			m_pBulletMeshComponent->GetMesh()->SetBoundBoxExtents(e * 0.5f, i);
		}
	}
	else if (m_eEnemyType == EnemyType::SpiralEnemy)
	{
		m_pBulletMeshComponent->GetMesh()->ChangeInstanceCount(100);
		m_pBullets->SetInstanceVector();
		m_pBullets->SetBulletType(2);
		auto instanceCount = m_pBulletMeshComponent->GetMesh()->GetInstanceCount();
		auto e = m_pBulletMeshComponent->GetMesh()->GetBoundBox(0).Extents;
		for (int i = 0; i < instanceCount; ++i)
		{
			m_pBulletMeshComponent->GetMesh()->SetPosition(GetTransform()->GetPosition(), i);
			m_pBulletMeshComponent->GetMesh()->SetScale(Vector3(1.0f, 1.0f, 1.0f), i);
			m_pBulletMeshComponent->GetMesh()->SetBoundBoxExtents(e * 0.5f, i);
		}
	}
	else if (m_eEnemyType == EnemyType::CoreEnemy)
	{
		auto r = GetComponent<MeshComponent>()->GetMesh()->GetBoundSphere().Radius;
		GetComponent<MeshComponent>()->GetMesh()->SetBoundSphereRadius(r * 3.0f);
		m_pBulletMeshComponent->GetMesh()->ChangeInstanceCount(300);
		m_pBullets->SetInstanceVector();
		m_pBullets->SetBulletType(1);
		auto instanceCount = m_pBulletMeshComponent->GetMesh()->GetInstanceCount();
		auto e = m_pBulletMeshComponent->GetMesh()->GetBoundBox(0).Extents;
		for (int i = 0; i < instanceCount; ++i)
		{
			m_pBulletMeshComponent->GetMesh()->SetPosition(GetTransform()->GetPosition(), i);
			m_pBulletMeshComponent->GetMesh()->SetScale(Vector3(1.0f, 1.0f, 1.0f), i);
			m_pBulletMeshComponent->GetMesh()->SetBoundBoxExtents(e * 0.5f, i);
		}
		m_HealthCount = 5;
	}
}

void Enemy::OnCreate()
{
	Entity::OnCreate();
}

void Enemy::OnUpdate(float deltaTime)
{
	Entity::OnUpdate(deltaTime);
	if (GetScene()->GetIsPause() || GetScene()->GetIsGameOver() || GetScene()->GetIsClear() || GetScene()->GetIsStart())
		return;
	m_DelayDamegeTime += deltaTime;

	if (m_eEnemyType == EnemyType::ChaseEnemy)
	{
		m_DelayFiringBulletTime += deltaTime;
		m_pBullets->GetComponent<TransformComponent>()->SetPosition(GetTransform()->GetPosition());
		m_pBullets->GetComponent<TransformComponent>()->SetRotation(GetTransform()->GetRotation());

		// 回転処理
		auto myPos = GetTransform()->GetPosition();
		auto dir = m_PlayerPos - myPos;
		dir.Normalize();
		Quaternion lookq = Quaternion::LookRotation(dir, m_worldUpward);
		auto q = GetTransform()->GetRotation();
		lookq = Quaternion::Lerp(lookq, q, m_moveValue * deltaTime);
		GetTransform()->SetRotation(lookq);
		m_pMeshComponent->GetMesh()->SetRotation(GetTransform()->GetRotation());

		//// 移動処理
		if (m_isMove)
		{
			dir = m_PlayerPos - myPos;
			dir.Normalize();
			auto pos = GetTransform()->GetPosition();
			pos += dir * m_moveValue * deltaTime;
			GetTransform()->SetPosition(pos);
			m_pMeshComponent->GetMesh()->SetPosition(GetTransform()->GetPosition());
		}

		// 敵の弾発射処理(プレイヤーに向かって打つ)
		if (m_DelayFiringBulletTime > 0.5f)
		{
			m_DelayFiringBulletTime = 0.0f;
			m_pBullets->SetActiveBulletIndex(m_BulletCount);
			if (m_BulletCount < m_pBullets->GetComponent<MeshComponent>()->GetMesh()->GetInstanceCount() - 1)
				++m_BulletCount;
			else
				m_BulletCount = 0;
		}
	}
	else if (m_eEnemyType == EnemyType::CircularEnemy)
	{
		m_DelayFiringBulletTime += deltaTime;
		m_pBullets->GetComponent<TransformComponent>()->SetPosition(GetTransform()->GetPosition());
		m_pBullets->GetComponent<TransformComponent>()->SetRotation(GetTransform()->GetRotation());

		// 弾発射処理
		if (m_DelayFiringBulletTime > 2.0f)
		{
			auto instanceCount = m_pBulletMeshComponent->GetMesh()->GetInstanceCount();
			for (int i = 0; i < 10; ++i)
			{
				m_pBullets->SetActiveBulletIndex(m_BulletCount);
				if (m_BulletCount < instanceCount - 1)
					++m_BulletCount;
				else
					m_BulletCount = 0;
			}
			m_DelayFiringBulletTime = 0.0f;
		}
	}
	else if (m_eEnemyType == EnemyType::SpiralEnemy)
	{
		m_DelayFiringBulletTime += deltaTime;
		m_pBullets->GetComponent<TransformComponent>()->SetPosition(GetTransform()->GetPosition());
		m_pBullets->GetComponent<TransformComponent>()->SetRotation(GetTransform()->GetRotation());

		// 敵の弾発射処理
		if (m_DelayFiringBulletTime > 0.2f)
		{
			m_DelayFiringBulletTime = 0.0f;
			m_pBullets->SetActiveBulletIndex(m_BulletCount);
			if (m_BulletCount < m_pBullets->GetComponent<MeshComponent>()->GetMesh()->GetInstanceCount() - 1)
				++m_BulletCount;
			else
				m_BulletCount = 0;
		}
	}
	else if (m_eEnemyType == EnemyType::CoreEnemy)
	{
		m_DelayFiringBulletTime += deltaTime;
		m_pBullets->GetComponent<TransformComponent>()->SetPosition(GetTransform()->GetPosition());
		m_pBullets->GetComponent<TransformComponent>()->SetRotation(GetTransform()->GetRotation());

		if (m_DelayFiringBulletTime > 0.3f)
		{
			for (int i = 0; i < 10; ++i)
			{
				m_pBullets->SetActiveBulletIndex(m_BulletCount);
				if (m_BulletCount < m_pBullets->GetComponent<MeshComponent>()->GetMesh()->GetInstanceCount() - 1)
					++m_BulletCount;
				else
					m_BulletCount = 0;
			}
			m_DelayFiringBulletTime = 0.0f;
		}

		if (m_HealthCount == 4)
		{
			m_pMeshComponent->GetMesh()->SetAllBaseColor(Vector3(1.0f, 1.0f, 1.0f));
		}
		else if (m_HealthCount == 3)
		{
			m_pMeshComponent->GetMesh()->SetAllBaseColor(Vector3(0.7f, 0.7f, 0.7f));
		}
		else if (m_HealthCount == 2)
		{
			m_pMeshComponent->GetMesh()->SetAllBaseColor(Vector3(0.4f, 0.4f, 0.4f));
		}
		else if (m_HealthCount == 1)
		{
			m_pMeshComponent->GetMesh()->SetAllBaseColor(Vector3(0.1f, 0.1f, 0.1f));
		}
	}
}
