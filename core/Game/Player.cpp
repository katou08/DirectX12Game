#include "Game/Player.h"
#include "Entity/Entity.h"
#include "Game/Bullet.h"
#include "Entity/MeshComponent.h"
#include "Game/Scene.h"
#include "Resource/Mesh.h"
#include "Entity/TransformComponent.h"
#include "Entity/CollisionTypeComponent.h"
#include "System/InputSystem.h"
#include "Game/Game.h"
#include "System/AudioSystem.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::TakeDamage()
{
	if (m_DelayDamegeTime > 0.5f)
	{
		--m_HealthCount;
		m_HealthCount = std::clamp(m_HealthCount, 0, 100);
		m_DelayDamegeTime = 0.0f;
		m_pScene->GetGame()->GetAudioSystem()->PlaySE(L"sound/playerDamage.wav", 0.1f);
	}
}

void Player::OnCreate()
{
	Entity::OnCreate();

	// 描画のためのメッシュコンポーネントを追加
	m_pPlayerMeshComponent = AddComponent<MeshComponent>();
	m_pPlayerMeshComponent->SetMesh(L"res/teapot/teapot.obj");

	m_pPlayerCollisionTypeComponent = AddComponent<CollisionTypeComponent>();
	m_pPlayerCollisionTypeComponent->SetCollisionType(CollisionType::PlayerCollision);

	GetTransform()->SetPosition(Vector3(0.0f, 0.0f, -3.0f));
	m_pPlayerMeshComponent->GetMesh()->SetPosition(GetTransform()->GetPosition());

	Quaternion q = Quaternion::CreateFromAxisAngle(m_worldUpward, DirectX::XM_PI);
	GetTransform()->SetRotation(q);
	m_pPlayerMeshComponent->GetMesh()->SetRotation(q);

	auto radius = m_pPlayerMeshComponent->GetMesh()->GetBoundSphere(0).Radius;
	m_pPlayerMeshComponent->GetMesh()->SetBoundSphereRadius(radius * 0.3f, 0);

	m_OldPos = GetTransform()->GetPosition();

	m_pBullets = m_pScene->AddEntity<Bullet>();
	m_pBulletMeshComponent = m_pBullets->GetComponent<MeshComponent>();
	m_pBullets->GetComponent<TransformComponent>()->SetPosition(GetTransform()->GetPosition());
	m_pBullets->GetComponent<TransformComponent>()->SetRotation(GetTransform()->GetRotation());

	m_pBulletMeshComponent->GetMesh()->ChangeInstanceCount(50);
	m_pBullets->SetInstanceVector();
	m_pBulletMeshComponent->SetActive(true);

	auto r = m_pBulletMeshComponent->GetMesh()->GetBoundSphere(0).Radius;
	auto instanceCount = m_pBulletMeshComponent->GetMesh()->GetInstanceCount();
	auto e = m_pBulletMeshComponent->GetMesh()->GetBoundBox(0).Extents;
	for (int i = 0; i < instanceCount; ++i)
	{
		m_pBulletMeshComponent->GetMesh()->SetPosition(GetTransform()->GetPosition(), i);
		m_pBulletMeshComponent->GetMesh()->SetScale(Vector3(1.0f, 1.0f, 1.0f), i);
		m_pBulletMeshComponent->GetMesh()->SetBoundSphereRadius(r * 2.0f, i);
		m_pBulletMeshComponent->GetMesh()->SetBoundBoxExtents(e * 0.5f, i);
	}
}

void Player::OnUpdate(float deltaTime)
{
	Entity::OnUpdate(deltaTime);
	if (GetScene()->GetIsPause() || GetScene()->GetIsGameOver() || GetScene()->GetIsClear() || GetScene()->GetIsStart())
		return;

	m_DelayFiring += deltaTime;
	m_DelayDamegeTime += deltaTime;
	auto kb = GetInputSystem()->GetKeybord()->GetState();
	auto mouse = GetInputSystem()->GetMouse()->GetState();

	m_forward = 0.0f;
	m_rightward = 0.0f;
	if (kb.D)
		m_rightward = 1.0f;
	if (kb.A)
		m_rightward = -1.0f;
	if (kb.W)
		m_forward = 1.0f;
	if (kb.S)
		m_forward = -1.0f;

	// 移動処理
	auto dir = m_worldForwardDir * m_forward + m_worldRightDir * m_rightward;
	dir.Normalize();
	auto transform = GetTransform();
	auto pos = transform->GetPosition();
	m_OldPos = pos;
	pos += dir * m_moveValue * deltaTime;
	pos.x = std::clamp(pos.x, -m_OtherBoundingBox.Extents.x, m_OtherBoundingBox.Extents.x);
	pos.z = std::clamp(pos.z, -m_OtherBoundingBox.Extents.z, m_OtherBoundingBox.Extents.z);
	transform->SetPosition(pos);
	m_pPlayerMeshComponent->GetMesh()->SetPosition(transform->GetPosition(), 0);

	// 回転処理
	if (m_rightward != 0.0f || m_forward != 0.0f)
	{
		auto q = transform->GetRotation();
		if(m_rightward != 0.0f)
			q = Quaternion::Lerp(q, Quaternion::CreateFromAxisAngle(m_worldUpward, DirectX::XM_PIDIV2 * -m_rightward), m_moveValue * deltaTime);
		if (m_forward == 1.0f)
			q = Quaternion::Lerp(q, Quaternion::CreateFromAxisAngle(m_worldUpward, DirectX::XM_PI * m_forward), m_moveValue * deltaTime);
		if(m_forward == -1.0f)
			q = Quaternion::Lerp(q, Quaternion::CreateFromAxisAngle(m_worldUpward, 0.0f), m_moveValue * deltaTime);
		transform->SetRotation(q);
		m_pPlayerMeshComponent->GetMesh()->SetRotation(transform->GetRotation(), 0);
	}

	if (m_HealthCount == 2)
	{
		m_pPlayerMeshComponent->GetMesh()->SetAllBaseColor(Vector3(0.3f, 0.0f, 0.0f));
	}
	else if (m_HealthCount == 1)
	{
		m_pPlayerMeshComponent->GetMesh()->SetAllBaseColor(Vector3(0.6f, 0.0f, 0.0f));
	}
	else if (m_HealthCount == 0)
	{
		m_pPlayerMeshComponent->GetMesh()->SetAllBaseColor(Vector3(1.0f, 0.0f, 0.0f));
	}

	// プレイヤーの弾発射処理
	if (mouse.leftButton)
	{
		if (m_DelayFiring > 0.1f)
		{
			++m_TotalFireBulletCount;
			m_pScene->GetGame()->GetAudioSystem()->PlaySE(L"sound/beam.wav", 0.1f);
			m_pBullets->SetActiveBulletIndex(m_BulletCount);
			if (m_BulletCount < m_pBullets->GetComponent<MeshComponent>()->GetMesh()->GetInstanceCount() - 1)
				++m_BulletCount;
			else
				m_BulletCount = 0;
			m_DelayFiring = 0.0f;
		}
	}

	// 弾のトランスフォームをプレイヤーのトランスフォームに移動
	m_pBullets->GetComponent<TransformComponent>()->SetPosition(transform->GetPosition());
	m_pBullets->GetComponent<TransformComponent>()->SetRotation(transform->GetRotation());
}