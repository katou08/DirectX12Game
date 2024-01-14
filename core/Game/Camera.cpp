#include "Game/Camera.h"
#include "Game/Game.h"
#include "Game/Display.h"
#include "pch.h"

Camera::Camera(Game* game) : m_pGame(game)
{
	m_Width = static_cast<float>(m_pGame->GetDisplay()->getClientSize().width);
	m_Height = static_cast<float>(m_pGame->GetDisplay()->getClientSize().height);
	m_Aspect = m_Width / m_Height;

	Update();
}

Camera::~Camera()
{
}

void Camera::Update()
{
	// �v���W�F�N�V�����s��̌v�Z
	if (m_ProjMove == ProjectionMode::Perspective)
		m_Proj = DirectX::XMMatrixPerspectiveFovLH(m_FovY, m_Aspect, m_Near, m_Far);
	else
		m_Proj = DirectX::XMMatrixOrthographicLH(m_Width, m_Height, m_Near, m_Far);

	// �r���[�s��̌v�Z
	m_View = DirectX::XMMatrixLookAtLH(m_Position, m_Target, m_Upward);
	// �r���[�t�s��̌v�Z
	m_ViewInv = DirectX::XMMatrixInverse(nullptr, m_View);

	// �J�����̑O�������̌v�Z
	m_Forward = Vector3(m_ViewInv.m[2][0], m_ViewInv.m[2][1], m_ViewInv.m[2][2]);
	// �J�����̉E�����̌v�Z
	m_Right = Vector3(m_ViewInv.m[0][0], m_ViewInv.m[0][1], m_ViewInv.m[0][2]);

	// �J�����̉�]�s����擾
	m_cameraRotation = m_ViewInv;
	m_cameraRotation.m[3][0] = 0.0f;
	m_cameraRotation.m[3][1] = 0.0f;
	m_cameraRotation.m[3][2] = 0.0f;
	m_cameraRotation.m[3][3] = 1.0f;

	//�����_�Ǝ��_�܂ŋ������v�Z
	Vector3 toPos;
	toPos = DirectX::XMVectorSubtract(m_Position, m_Target);
	m_targetToPositionLen = toPos.Length();

	m_IsDirty = false;
}

//! @brief �����_�����_�Ƃ��ăJ��������]������
//! @param[in] ��]������N�H�[�^�j�I��
void Camera::RotateOriginTarget(const Quaternion& qRot)
{
	Vector3 cameraPos = m_Position;
	Vector3 cameraTarget = m_Target;
	Vector3 toPos = DirectX::XMVectorSubtract(cameraPos, cameraTarget);
	toPos = DirectX::XMVector3Rotate(toPos, qRot);

	m_Position = m_Target + toPos;
	m_IsDirty = true;
}

//! @brief �J�����𓮂���
//! @param[in] �ړ���
void Camera::Move(const Vector3& move)
{
	m_Position += move;
	m_Target += move;
	m_IsDirty = true;
}

//! @brief �����_�𓮂���
//! @param[in] �ړ���
void Camera::MoveTarget(const Vector3& move)
{
	m_Target += move;
	m_IsDirty = true;
}

//! @brief ���_�𓮂���
//! @param[in] �ړ���
void Camera::MovePosition(const Vector3& move)
{
	m_Position += move;
	m_IsDirty = true;
}

//! @brief �J�����̑O�������Ɉړ�
void Camera::MoveForward(float moveForward)
{
	auto forward = DirectX::XMVectorScale(m_Forward, moveForward);
	Move(forward);
}

void Camera::MoveRight(float moveRight)
{
	auto right = DirectX::XMVectorScale(m_Right, moveRight);
	Move(right);
}

void Camera::MoveUp(float moveUp)
{
	auto upward = DirectX::XMVectorScale(m_Upward, moveUp);
	Move(upward);
}

void Camera::Reset()
{
	SetPositionAndTarget({ 0.0f, 0.0f, -5.0f }, { 0.0f, 0.0f, 0.0f });
}

//! @brief �J�����̈ʒu��ݒ�
void Camera::SetPosition(const Vector3& pos)
{
	m_Position = pos;
	m_IsDirty = true;
}

void Camera::SetPosition(float x, float y, float z)
{
	SetPosition({ x, y, z });
}

void Camera::SetPositionAndTarget(const Vector3& pos, const Vector3& target)
{
	m_Position = pos;
	m_Target   = target;
	m_IsDirty  = true;
}

const Matrix& Camera::GetView()
{
	if (m_IsDirty)
		Update();

	return m_View;
}

const Matrix& Camera::GetProj()
{
	if (m_IsDirty)
		Update();

	return m_Proj;
}

const Matrix& Camera::GetViewProj()
{
	if (m_IsDirty)
		Update();

	return m_View * m_Proj;
}

const Matrix& Camera::GetViewInv()
{
	if (m_IsDirty)
		Update();

	return m_ViewInv;
}
