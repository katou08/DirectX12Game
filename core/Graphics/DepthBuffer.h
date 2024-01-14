#pragma once
#include "pch.h"

class DepthBuffer
{
public:
	DepthBuffer() {};
	~DepthBuffer() {};
	bool Create(ID3D12Device* pDevice, uint32_t width, uint32_t height,
		DXGI_FORMAT format, float clearDepth, uint8_t clearStencil);
	ID3D12Resource* GetResource() const { return m_pBuffer.Get(); }
	void Release();

private:
	ComPtr<ID3D12Resource> m_pBuffer = nullptr;

	DepthBuffer(const DepthBuffer&) = delete;
	void operator = (const DepthBuffer&) = delete;
};