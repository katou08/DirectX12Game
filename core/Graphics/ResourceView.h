#pragma once
#include "pch.h"

class ResourceView
{
public:
	inline uint32_t GetSize() const { return m_size; }
	inline D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescHandle(uint32_t i = 0) const 
		{ return D3D12_CPU_DESCRIPTOR_HANDLE{ m_CPUHandle.ptr + static_cast<uint64_t>(i) * m_DescriptorSize }; }
	inline D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescHandle(uint32_t i = 0) const
		{ return D3D12_GPU_DESCRIPTOR_HANDLE{ m_GPUHandle.ptr + static_cast<uint64_t>(i) * m_DescriptorSize }; }

	inline void SetResourceView(uint32_t size, uint32_t descriptorSize, D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle, D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle)
	{
		m_size			 = size;
		m_CPUHandle		 = cpuHandle;
		m_GPUHandle		 = gpuHandle;
		m_DescriptorSize = descriptorSize;
	}

private:
	uint32_t m_size = 0;			//!< 作成したビューの数(ビューごとのオフセットの計算に使用)
	uint32_t m_DescriptorSize = 0;  //!< リソースビューごとのハンドルのインクリメントサイズ

	D3D12_CPU_DESCRIPTOR_HANDLE m_CPUHandle;
	D3D12_GPU_DESCRIPTOR_HANDLE m_GPUHandle;
};

class RenderTargetView : public ResourceView{};
class DepthStencilView : public ResourceView{};
class ConstantBufferView : public ResourceView{};
class ShaderResourceView : public ResourceView{};