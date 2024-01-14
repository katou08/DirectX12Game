#pragma once
#include "pch.h"

// 一つのメッシュのインスタンス最大数
enum InstCount
{
	InstanceDataMax = 500,
};

// ワールド行列
struct alignas(256) WorldTransform
{
	Matrix WorldInstanceData[InstanceDataMax];
};

// ビュー行列, プロジェクション行列
struct alignas(256) ViewProjTransform
{
	Matrix View;
	Matrix Proj;
};

// ライト
struct alignas(256) LightBuffer
{
	Vector4 LightPosition;	//!< ライト位置
	Color	LightColor;		//!< ライトカラー
	Vector4 CameraPosition; //!< カメラ位置
};

// マテリアル
struct alignas(256) MaterialBuffer
{
	Vector3 Difuuse;	//!< 基本色
	float Alpha;		//!< 透過成分
	Vector3 Specular;	//!< 鏡面反射
	float Shininess;	//!< 鏡面反射強度
};