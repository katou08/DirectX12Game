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

//! @brief �������Ԃ��߂������C�Փ˂����e�����̈ʒu�ɖ߂�
//! @param[in] index �e�̔ԍ�
void Bullet::SetDeActiveBulletIndex(const uint32_t& index)
{
	m_isActiveBullet[index] = false;
	m_pMeshComponent->GetMesh()->SetPosition(GetComponent<TransformComponent>()->GetPosition(), index);
	m_FirstBulletFireDir[index] = Vector3(0.0f, 0.0f, 0.0f);
	m_survTimes[index] = 0.0f;
}

//! @brief �C���X�^���X�̐����ϐ��̃T�C�Y�����T�C�Y
void Bullet::SetInstanceVector()
{
	auto instanceCount = GetComponent<MeshComponent>()->GetMesh()->GetInstanceCount();
	m_isActiveBullet.resize(instanceCount);
	m_survTimes.resize(instanceCount);
	m_FirstBulletFireDir.resize(instanceCount);
}

//! @brief ��������
void Bullet::OnCreate()
{
	Entity::OnCreate();

	// �`��̂��߂̃��b�V���R���|�[�l���g��ǉ�
	m_pMeshComponent = AddComponent<MeshComponent>();
	m_pMeshComponent->SetMesh(L"res/box/box.obj");
	m_pBulletCollisionTypeComponent = AddComponent<CollisionTypeComponent>();
	m_pBulletCollisionTypeComponent->SetCollisionType(CollisionType::BulletCollision);

	auto instanceCount = GetComponent<MeshComponent>()->GetMesh()->GetInstanceCount();
	m_isActiveBullet.resize(instanceCount);
	m_survTimes.resize(instanceCount);
	m_FirstBulletFireDir.resize(instanceCount);
}

//! @brief �X�V����
//! @param[in] deltaTime �f���^�^�C��
void Bullet::OnUpdate(float deltaTime)
{
	Entity::OnUpdate(deltaTime);
	if (GetScene()->GetIsPause() || GetScene()->GetIsGameOver() || GetScene()->GetIsClear() || GetScene()->GetIsStart())
		return;

	// ���i����e�̏ꍇ
	if (m_eBulletType == BulletType::NormalBullet)
	{
		// ���˂��ꂽ�e�̂�
		for (int i = 0; i < m_isActiveBullet.size(); ++i)
		{
			if (m_isActiveBullet[i] == true)
			{
				m_survTimes[i] += deltaTime;
				// �ŏ��ɔ��˂���e�̕��������߂�
				if (m_FirstBulletFireDir[i] == Vector3(0.0f, 0.0f, 0.0f))
				{
					m_FirstBulletFireDir[i] = GetTransform()->GetWorldTransform().Forward();
					m_FirstBulletFireDir[i].Normalize();
				}
				auto pos = GetComponent<MeshComponent>()->GetMesh()->GetPosition(i);
				pos += m_FirstBulletFireDir[i] * m_moveValue * deltaTime;
				// �}�b�v�O�ɏo����
				if (pos.x < -m_OtherBoundingBox.Extents.x || pos.x > m_OtherBoundingBox.Extents.x ||
					pos.z < -m_OtherBoundingBox.Extents.z || pos.z > m_OtherBoundingBox.Extents.z)
				{
					SetDeActiveBulletIndex(i);
				}
				m_pMeshComponent->GetMesh()->SetPosition(pos, i);
				// �������Ԃ��߂�����
				if (m_survTimes[i] > 1.0f)
					SetDeActiveBulletIndex(i);
			}
			else
				m_pMeshComponent->GetMesh()->SetPosition(GetComponent<TransformComponent>()->GetPosition(), i);
		}
	}
	// �S���ʂ̒e�̏ꍇ
	else if (m_eBulletType == BulletType::CircularBullet)
	{
		// ���˂��ꂽ�e�̂�
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

	// �x�N�g������]�s��ŕϊ�
	Vector3 rotated = XMVector3Transform(originalVector, rotationMatrix);

	return rotated;
}