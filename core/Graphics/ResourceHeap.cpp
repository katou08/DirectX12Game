#include "Graphics/ResourceHeap.h"
#include "Graphics/ResourceView.h"
#include "pch.h"
#include <cassert>

//! @brief ディスクリプタの作成
//! @param[in] pDevice			デバイス
//! @param[in] resourceName		リソース名
//! @param[in] heapType			ヒープの種類
//! @param[in] descriptorCount  ディスクリプタの数
//! @param[in] forceCPUVisible  ディスクリプタをCPUからのアクセスできるようにするかどうか
//! @return ディスクリプタの作成に成功したか
void ResourceHeap::Create(ID3D12Device* pDevice, const std::string& resourceName, D3D12_DESCRIPTOR_HEAP_TYPE heapType, uint32_t descriptorCount, bool forceCPUVisible)
{
	this->m_descriptorCount = descriptorCount; //!< 最大ディスクリプタ数を指定
	this->m_index = 0;
	this->m_DescriptorElementSize = pDevice->GetDescriptorHandleIncrementSize(heapType);

	// ディスクリプタヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = descriptorCount;
	heapDesc.Type = heapType;
	heapDesc.NodeMask = 0;
	heapDesc.Flags = ((heapType == D3D12_DESCRIPTOR_HEAP_TYPE_RTV) || (heapType == D3D12_DESCRIPTOR_HEAP_TYPE_DSV))
		? D3D12_DESCRIPTOR_HEAP_FLAG_NONE
		: D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	if (forceCPUVisible)
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	// ディスクリプタヒープの作成
	pDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_pHeap.GetAddressOf()));
	this->m_bGPUVisibile = heapDesc.Flags == D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	this->m_pHeap->SetName(Utility::StringToWString(resourceName).c_str());
}

//! @brief ディスクリプタの割り当て
//! @param[in] size  サイズ
//! @param[in] pRV	 リソースビュー
//! @return ディスクリプタの割り当てに成功したか
bool ResourceHeap::AllocDescriptor(uint32_t size, ResourceView* pRV)
{
	if ((m_index + size) > m_descriptorCount) // 指定した最大ディスクリプタ数より上回っていた場合
	{
		ThrowFailed("ディスクリプタヒープがメモリ不足です.");
		return false;
	}

	// ディスクリプタのCPU, CPUハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle = m_pHeap->GetCPUDescriptorHandleForHeapStart();
	CPUHandle.ptr += m_index * m_DescriptorElementSize;

	D3D12_GPU_DESCRIPTOR_HANDLE GPUHandle = m_bGPUVisibile
		? m_pHeap->GetGPUDescriptorHandleForHeapStart() : D3D12_GPU_DESCRIPTOR_HANDLE{};
	GPUHandle.ptr += m_bGPUVisibile ? m_index * m_DescriptorElementSize : GPUHandle.ptr;

	m_index += size;

	pRV->SetResourceView(size, m_DescriptorElementSize, CPUHandle, GPUHandle);

	return true;
}

void ResourceHeap::Destroy()
{
	m_pHeap->Release();
}
