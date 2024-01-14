#pragma once
#include "pch.h"

class VertexBuffer
{
public:
	VertexBuffer();
	~VertexBuffer();

	bool Init(ID3D12Device* pDevice, size_t size, size_t stride, const void* pInitData = nullptr);

	template<typename T>
	bool Init(ID3D12Device* pDevice, size_t count, const T* pInitData = nullptr)
	{
		return Init(pDevice, count, sizeof(T), pInitData);
	}

	void* Map() const;

	template<typename T>
	T* Map() const
	{
		return reinterpret_cast<T*>(Map());
	}

	void Unmap();
	D3D12_VERTEX_BUFFER_VIEW GetView() const { return m_View; }

private:
	ComPtr<ID3D12Resource> m_pVB;
	D3D12_VERTEX_BUFFER_VIEW m_View;
};