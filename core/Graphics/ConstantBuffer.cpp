#include "Graphics/ConstantBuffer.h"

ConstantBuffer::ConstantBuffer()
{

}

ConstantBuffer::~ConstantBuffer()
{

}

//! @brief CB�̍쐬
//! @param[in] pDevice  �f�o�C�X
//! @param[in] size		�f�[�^�̃T�C�Y
//! @param[in] name		CB�̖��O
//! @param[in] srcData  �}�b�s���O����o�b�t�@�̃|�C���^
//! @return CB�̍쐬�ɐ���������
bool ConstantBuffer::Init(ID3D12Device* pDevice, uint32_t size, const std::string& name, void* srcData)
{
	if (pDevice == nullptr || size == 0)
		return false;

	// �q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES prop = {};
	prop.Type				  = D3D12_HEAP_TYPE_UPLOAD;
	prop.CPUPageProperty	  = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask	  = 1;
	prop.VisibleNodeMask	  = 1;

	//�萔�o�b�t�@��256�o�C�g�A���C�����g���v�������̂ŁA256�̔{���ɐ؂�グ��
	m_allocSize = (size + 256) & 0xFFFFFF00;

	// ���\�[�X�̐ݒ�
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension			= D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Alignment			= 0;
	resDesc.Width				= static_cast<UINT64>(m_allocSize);
	resDesc.Height				= 1;
	resDesc.DepthOrArraySize	= 1;
	resDesc.MipLevels			= 1;
	resDesc.Format				= DXGI_FORMAT_UNKNOWN;
	resDesc.SampleDesc.Count	= 1;
	resDesc.SampleDesc.Quality	= 0;
	resDesc.Layout				= D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resDesc.Flags				= D3D12_RESOURCE_FLAG_NONE;

	// �萔�o�b�t�@�̍쐬
	auto hr = pDevice->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_pCB.GetAddressOf()));
	ThrowIfFailed(hr, "�R���X�^���g�o�b�t�@�̍쐬�Ɏ��s���܂���.");
	m_pCB->SetName(Utility::StringToWString(name).c_str());

	//�萔�o�b�t�@��CPU����A�N�Z�X�\�ȉ��z�A�h���X��ԂɃ}�b�s���O����
	D3D12_RANGE	readRange = { 0, 0 };		// CPU����o�b�t�@��ǂݍ��܂Ȃ��ݒ�
	hr = m_pCB->Map(0, &readRange, &m_pMappedPtr);
	if (FAILED(hr))
		return false;

	m_Desc.BufferLocation = m_pCB->GetGPUVirtualAddress();
	m_Desc.SizeInBytes	  = static_cast<UINT>(m_allocSize);

	return true;
}
