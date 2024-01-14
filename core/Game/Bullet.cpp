#include "Game/Bullet.h"
#include "Entity/Entity.h"
#include "Entity/MeshComponent.h"
#include "Resource/Mesh.h"
#include "System/InputSystem.h"
#include "Game/Scene.h"
#include "Entity/TransformComponent.h"
#include "Entity/CollisionTypeComponent.h"

Bullet::Bullet()
{
}

Bullet::~Bullet()
{
}

//! @brief 生存時間が過ぎたか，衝突した弾を元の位置に戻す
//! @param[in] index 弾の番号
void Bullet::SetDeActiveBulletIndex(const uint32_t& index)
{
	m_isActiveBullet[index] = false;
	m_pMeshComponent->GetMesh()->SetPosition(GetComponent<TransformComponent>()->GetPosition(), index);
	m_FirstBulletFireDir[index] = Vector3(0.0f, 0.0f, 0.0f);
	m_survTimes[index] = 0.0f;
}

//! @brief インスタンスの数分変数のサイズをリサイズ
void Bullet::SetInstanceVector()
{
	auto instanceCount = GetComponent<MeshComponent>()->GetMesh()->GetInstanceCount();
	m_isActiveBullet.resize(instanceCount);
	m_survTimes.resize(instanceCount);
	m_FirstBulletFireDir.resize(instanceCount);
}

//! @brief 生成処理
void Bullet::OnCreate()
{
	Entity::OnCreate();

	// 描画のためのメッシュコンポーネントを追加
	m_pMeshComponent = AddComponent<MeshComponent>();
	m_pMeshComponent->SetMesh(L"res/box/box.obj");
	m_pBulletCollisionTypeComponent = AddComponent<CollisionTypeComponent>();
	m_pBulletCollisionTypeComponent->SetCollisionType(CollisionType::BulletCollision);

	auto instanceCount = GetComponent<MeshComponent>()->GetMesh()->GetInstanceCount();
	m_isActiveBullet.resize(instanceCount);
	m_survTimes.resize(instanceCount);
	m_FirstBulletFireDir.resize(instanceCount);
}

//! @brief 更新処理
//! @param[in] deltaTime デルタタイム
void Bullet::OnUpdate(float deltaTime)
{
	Entity::OnUpdate(deltaTime);
	if (GetScene()->GetIsPause() || GetScene()->GetIsGameOver() || GetScene()->GetIsClear() || GetScene()->GetIsStart())
		return;

	// 直進する弾の場合
	if (m_eBulletType == BulletType::NormalBullet)
	{
		// 発射された弾のみ
		for (int i = 0; i < m_isActiveBullet.size(); ++i)
		{
			if (m_isActiveBullet[i] == true)
			{
				m_survTimes[i] += deltaTime;
				// 最初に発射する弾の方向を決める
				if (m_FirstBulletFireDir[i] == Vector3(0.0f, 0.0f, 0.0f))
				{
					m_FirstBulletFireDir[i] = GetTransform()->GetWorldTransform().Forward();
					m_FirstBulletFireDir[i].Normalize();
				}
				auto pos = GetComponent<MeshComponent>()->GetMesh()->GetPosition(i);
				pos += m_FirstBulletFireDir[i] * m_moveValue * deltaTime;
				// マップ外に出たら
				if (pos.x < -m_OtherBoundingBox.Extents.x || pos.x > m_OtherBoundingBox.Extents.x ||
					pos.z < -m_OtherBoundingBox.Extents.z || pos.z > m_OtherBoundingBox.Extents.z)
				{
					SetDeActiveBulletIndex(i);
				}
				m_pMeshComponent->GetMesh()->SetPosition(pos, i);
				// 生存時間を過ぎたら
				if (m_survTimes[i] > 1.0f)
					SetDeActiveBulletIndex(i);
			}
			else
				m_pMeshComponent->GetMesh()->SetPosition(GetComponent<TransformComponent>()->GetPosition(), i);
		}
	}
	// 全方位の弾の場合
	else if (m_eBulletType == BulletType::CircularBullet)
	{
		// 発射された弾のみ
		for (int i = 0; i < m_isActiveBullet.size(); ++i)
		{
			if (m_isActiveBullet[i] == true)
			{
				m_survTimes[i] += deltaTime;
				auto dir = GetTransform()->GetWorldTransform().Forward();
				dir = RotateVectorAroundY(dir, i * 0.628f);
				dir = RotateVectorAroundY(dir, (DirectX::XM_PIDIV2 / 10) * m_survTimes[i]);
				dir.Normalize();
				auto pos = GetComponent<MeshComponent>()->GetMesh()->GetPosition(i);
				int movevalue = 5.0f;
				pos += dir * movevalue * deltaTime;
				if (pos.x < -m_OtherBoundingBox.Extents.x || pos.x > m_OtherBoundingBox.Extents.x ||
					pos.z < -m_OtherBoundingBox.Extents.z || pos.z > m_OtherBoundingBox.Extents.z)
				{
					SetDeActiveBulletIndex(i);
				}
				m_pMeshComponent->GetMesh()->SetPosition(pos, i);
				if (m_survTimes[i] > 5.0f)
					SetDeActiveBulletIndex(i);
			}
			else
				m_pMeshComponent->GetMesh()->SetPosition(GetComponent<TransformComponent>()->GetPosition(), i);
		}
	}
	else if (m_eBulletType == BulletType::SpiralBullet)
	{
		for (int i = 0; i < m_isActiveBullet.size(); ++i)
		{
			if (m_isActiveBullet[i] == true)
			{
				m_survTimes[i] += deltaTime;
				auto dir = GetTransform()->GetWorldTransform().Forward();
				dir.Normalize();
				auto pos = GetComponent<MeshComponent>()->GetMesh()->GetPosition(i);
				int movevalue = 5.0f;
				pos += dir * movevalue * deltaTime;
				if (pos.x < -m_OtherBoundingBox.Extents.x || pos.x > m_OtherBoundingBox.Extents.x ||
					pos.z < -m_OtherBoundingBox.Extents.z || pos.z > m_OtherBoundingBox.Extents.z)
					SetDeActiveBulletIndex(i);
				m_pMeshComponent->GetMesh()->SetPosition(pos, i);
				if (m_survTimes[i] > 5.0f)
					SetDeActiveBulletIndex(i);
			}
			else
				m_pMeshComponent->GetMesh()->SetWorldTransform(GetComponent<TransformComponent>()->GetWorldTransform(), i);
		}
	}
}

const Vector3& Bullet::RotateVectorAroundY(const Vector3& originalVector, float angleInRadians)
{
	DirectX::XMMATRIX rotationMatrix = DirectX::XMMatrixRotationY(angleInRadians);

	// ベクトルを回転行列で変換
	Vector3 rotated = XMVector3Transform(originalVector, rotationMatrix);

	return rotated;
}