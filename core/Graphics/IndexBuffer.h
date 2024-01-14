#pragma once
#include "pch.h"

class IndexBuffer
{
public:
	IndexBuffer();
	~IndexBuffer();

	bool Init(ID3D12Device* pDevice, size_t count, const void* pInitData = nullptr);

	void* Map() const;

	template<typename T>
	T* Map() const
	{
		return reinterpret_cast<T*>(Map());
	}

	void Unmap();
	D3D12_INDEX_BUFFER_VIEW GetView() const { return m_View; }
	size_t GetCount() const { return m_Count; };

private:
	ComPtr<ID3D12Resource> m_pIB;
	D3D12_INDEX_BUFFER_VIEW m_View;
	size_t m_Count; //!< インデックス数
};