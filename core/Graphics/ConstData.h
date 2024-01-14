#pragma once
#include "pch.h"

// ��̃��b�V���̃C���X�^���X�ő吔
enum InstCount
{
	InstanceDataMax = 500,
};

// ���[���h�s��
struct alignas(256) WorldTransform
{
	Matrix WorldInstanceData[InstanceDataMax];
};

// �r���[�s��, �v���W�F�N�V�����s��
struct alignas(256) ViewProjTransform
{
	Matrix View;
	Matrix Proj;
};

// ���C�g
struct alignas(256) LightBuffer
{
	Vector4 LightPosition;	//!< ���C�g�ʒu
	Color	LightColor;		//!< ���C�g�J���[
	Vector4 CameraPosition; //!< �J�����ʒu
};

// �}�e���A��
struct alignas(256) MaterialBuffer
{
	Vector3 Difuuse;	//!< ��{�F
	float Alpha;		//!< ���ߐ���
	Vector3 Specular;	//!< ���ʔ���
	float Shininess;	//!< ���ʔ��ˋ��x
};