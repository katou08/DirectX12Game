#pragma once
#include "pch.h"

class Game;

class Camera
{
public:
	enum ProjectionMode
	{
		Perspective, //! �������e
		Orhographic, //! ���ˉe
	};

	Camera(Game* game);
	~Camera();
	void Update();
	void RotateOriginTarget(const Quaternion& qRot);
	void Move(const Vector3& move);
	void MoveTarget(const Vector3& move);
	void MovePosition(const Vector3& move);
	void MoveForward(float moveForward);
	void MoveRight(float moveRight);
	void MoveUp(float moveUp);
	void Reset();

	void SetPosition(const Vector3& pos);
	void SetPosition(float x, float y, float z);
	void SetPositionAndTarget(const Vector3& pos, const Vector3& target);

	const Vector3& GetPosition() const { return m_Position; }
	const Vector3& GetTarget() const { return m_Target; }
	const Vector3& GetForward() const { return m_Forward; }
	const Vector3& GetRight() const { return m_Right; }
	const Matrix& GetView();
	const Matrix& GetProj();
	const Matrix& GetViewProj();
	const Matrix& GetViewInv();

private:
	Game* m_pGame = nullptr;

	Vector3 m_Position = { 0.0f, 0.0f, -5.0f }; //!< �J�����ʒu
	Vector3 m_Target = { 0.0f, 0.0f, 0.0f }; //!< �J�����̒����_
	Vector3 m_Upward = { 0.0f, 1.0f, 0.0f }; //!< �J�����̏����
	Matrix m_View = Matrix::Identity; //!< �r���[�s��
	Matrix m_ViewInv = Matrix::Identity; //!< �r���[�s��̋t�s��
	Matrix m_Proj = Matrix::Identity;//!< �v���W�F�N�V�����s��
	Matrix m_cameraRotation = Matrix::Identity; //!< �J�����̉�]�s��
	Vector3 m_Forward = { 0.0f, 0.0f, 1.0f }; //!< �J�����̑O������
	Vector3 m_Right = { 1.0f, 0.0f, 0.0f }; //!< �J�����̉E����
	float m_targetToPositionLen = 1.0f;	//�����_�Ǝ��_�܂ŋ���

	float m_Near = 1.0f;
	float m_Far = 10000.0f;
	float m_FovY = DirectX::XMConvertToRadians(60.0f);
	float m_Aspect = 1.0f;
	float m_Width = 1.0f;
	float m_Height = 1.0f;
	ProjectionMode m_ProjMove = ProjectionMode::Perspective;
	bool m_IsDirty = false;
};