#include "Graphics/ResourceHeap.h"
#include "Graphics/ResourceView.h"
#include "pch.h"
#include <cassert>

//! @brief �f�B�X�N���v�^�̍쐬
//! @param[in] pDevice			�f�o�C�X
//! @param[in] resourceName		���\�[�X��
//! @param[in] heapType			�q�[�v�̎��
//! @param[in] descriptorCount  �f�B�X�N���v�^�̐�
//! @param[in] forceCPUVisible  �f�B�X�N���v�^��CPU����̃A�N�Z�X�ł���悤�ɂ��邩�ǂ���
//! @return �f�B�X�N���v�^�̍쐬�ɐ���������
void ResourceHeap::Create(ID3D12Device* pDevice, const std::string& resourceName, D3D12_DESCRIPTOR_HEAP_TYPE heapType, uint32_t descriptorCount, bool forceCPUVisible)
{
	this->m_descriptorCount = descriptorCount; //!< �ő�f�B�X�N���v�^�����w��
	this->m_index = 0;
	this->m_DescriptorElementSize = pDevice->GetDescriptorHandleIncrementSize(heapType);

	// �f�B�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = descriptorCount;
	heapDesc.Type = heapType;
	heapDesc.NodeMask = 0;
	heapDesc.Flags = ((heapType == D3D12_DESCRIPTOR_HEAP_TYPE_RTV) || (heapType == D3D12_DESCRIPTOR_HEAP_TYPE_DSV))
		? D3D12_DESCRIPTOR_HEAP_FLAG_NONE
		: D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	if (forceCPUVisible)
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	// �f�B�X�N���v�^�q�[�v�̍쐬
	pDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(m_pHeap.GetAddressOf()));
	this->m_bGPUVisibile = heapDesc.Flags == D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	this->m_pHeap->SetName(Utility::StringToWString(resourceName).c_str());
}

//! @brief �f�B�X�N���v�^�̊��蓖��
//! @param[in] size  �T�C�Y
//! @param[in] pRV	 ���\�[�X�r���[
//! @return �f�B�X�N���v�^�̊��蓖�Ăɐ���������
bool ResourceHeap::AllocDescriptor(uint32_t size, ResourceView* pRV)
{
	if ((m_index + size) > m_descriptorCount) // �w�肵���ő�f�B�X�N���v�^���������Ă����ꍇ
	{
		ThrowFailed("�f�B�X�N���v�^�q�[�v���������s���ł�.");
		return false;
	}

	// �f�B�X�N���v�^��CPU, CPU�n���h�����擾
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
