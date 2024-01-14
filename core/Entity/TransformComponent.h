#pragma once
#include "pch.h"
#include "Entity/Component.h"
#include "Entity/MeshComponent.h"
#include "Resource/Mesh.h"

class TransformComponent : public Component
{
public:
	TransformComponent();
	virtual ~TransformComponent();

	const Vector3& GetPosition() const { return m_Position; }
	void SetPosition(const Vector3& pos)
	{
		m_Position = pos;
		ComputeWorldTransform();
	}

	const Vector3& GetScale() const { return m_Scale; }
	void SetScale(const Vector3& scale)
	{
		m_Scale = scale;
		ComputeWorldTransform();
	}

	const Quaternion& GetRotation() const { return m_Rotation; }
	void SetRotation(const Quaternion& rotation)
	{
		m_Rotation = rotation;
		ComputeWorldTransform();
	}

	const Matrix& GetWorldTransform() const { return m_WorldTransform; }
	void SetWorldTransform(const Matrix& world)
	{
		m_WorldTransform = world;
	}

protected:
	void ComputeWorldTransform();
	
	// ç¿ïWïœä∑
	Matrix m_WorldTransform = Matrix::Identity;

	Vector3 m_Position		= Vector3::Zero;
	Quaternion m_Rotation	= Quaternion::Identity;
	Vector3 m_Scale			= Vector3::One;
};