#pragma once
#include "pch.h"

class ConstantBuffer
{
public:
	ConstantBuffer();
	~ConstantBuffer();

	bool Init(ID3D12Device* pDevice, uint32_t size, const std::string& name, void* srcData = nullptr);
	D3D12_GPU_VIRTUAL_ADDRESS GetAddress() const { return m_Desc.BufferLocation; }
	void* GetPtr() const { return m_pMappedPtr; }
	D3D12_CONSTANT_BUFFER_VIEW_DESC GetDesc() const { return m_Desc; }

	//! @brief �������}�b�s���O�ς݂̃|�C���^���擾
	template<typename T>
	T* GetPtr()
	{
		return reinterpret_cast<T*>(GetPtr());
	}
private:
	ComPtr<ID3D12Resource> m_pCB = nullptr;
	uint32_t m_allocSize = 0;						//!< �o�b�t�@�̃T�C�Y 256�ɃA���C�����g
	D3D12_CONSTANT_BUFFER_VIEW_DESC m_Desc = {};	//!< �萔�o�b�t�@�r���[�̍\���ݒ�
	void* m_pMappedPtr = nullptr;					//!< CPU������A�N�Z�X����萔�o�b�t�@�̃A�h���X
	
};