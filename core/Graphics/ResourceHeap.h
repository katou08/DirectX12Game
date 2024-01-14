#pragma once
#include "pch.h"

class ResourceView;

enum EResourceHeapType
{
	RTV_HEAP = 0,
	DSV_HEAP,
	CBC_SRV_UAV_HEAP,
	SAMPLER_HEAP,

	NUM_HEAP_TYPES
};

class ResourceHeap
{
public:
	void Create(ID3D12Device* pDevice, 
		const std::string& resourceName, 
		D3D12_DESCRIPTOR_HEAP_TYPE heapType,
		uint32_t descriptorCount, 
		bool forceCPUVisible = false);
	bool AllocDescriptor(uint32_t size, ResourceView* pRV);
	void Destroy();

	inline ID3D12DescriptorHeap* GetHeap() const { return m_pHeap.Get(); }

private:
	uint32_t m_index;
	uint32_t m_descriptorCount;
	uint32_t m_DescriptorElementSize;

	ComPtr<ID3D12DescriptorHeap> m_pHeap;
	bool m_bGPUVisibile;
};