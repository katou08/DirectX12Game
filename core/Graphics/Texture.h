#pragma once
#include "pch.h"

class Texture
{
public:
	Texture() {};

	bool Init(ID3D12Device* pDevice,
		const wchar_t* filename, bool isSRGB, DirectX::ResourceUploadBatch& batch,
		bool isCube);

	bool Init(ID3D12Device* pDevice,
		const D3D12_RESOURCE_DESC* pDesc, bool isSRGB, bool isCube);

	ID3D12Resource* GetBuffer() const { return m_pBuffer.Get(); }
	D3D12_SHADER_RESOURCE_VIEW_DESC GetDesc() const { return m_Desc; }

private:
	D3D12_SHADER_RESOURCE_VIEW_DESC GetViewDesc(bool isCube);

	ComPtr<ID3D12Resource> m_pBuffer = nullptr;
	D3D12_SHADER_RESOURCE_VIEW_DESC m_Desc = {};
	Texture(const Texture&) = delete;
	void operator = (const Texture&) = delete;
};