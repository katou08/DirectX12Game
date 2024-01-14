#include "Graphics/IndexBuffer.h"

//! @brief �R���X�g���N�^
IndexBuffer::IndexBuffer()
	:m_pIB(nullptr)
	,m_Count(0)
{
	memset(&m_View, 0, sizeof(m_View));
}

//! @brief �f�X�g���N�^
IndexBuffer::~IndexBuffer()
{
	m_pIB.Reset();
	memset(&m_View, 0, sizeof(m_View));
}

//! @brief �C���f�b�N�X�o�b�t�@�̍쐬
//! @param[in] pDevice		�f�o�C�X
//! @param[in] count		�o�b�t�@�̑傫��
//! @param[in] pInitData	�}�b�s���O����o�b�t�@�̃|�C���^
//! @return �C���f�b�N�X�o�b�t�@�̍쐬�ɐ���������
bool IndexBuffer::Init(ID3D12Device* pDevice, size_t count, const void* pInitData)
{
	// �q�[�v�v���p�e�B
	D3D12_HEAP_PROPERTIES prop = {};
	prop.Type					= D3D12_HEAP_TYPE_UPLOAD; //! GPU�ɓ]�����邽�߂̃q�[�v
	prop.CPUPageProperty		= D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference	= D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask		= 1; //! �P���GPU��O��̂���1
	prop.VisibleNodeMask		= 1; //! �P���GPU��O��̂���1

	// ���\�[�X�̐ݒ�
	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension			= D3D12_RESOURCE_DIMENSION_BUFFER; //! �o�b�t�@
	desc.Alignment			= 0;
	desc.Width				= static_cast<UINT64>(count);
	desc.Height				= 1;
	desc.DepthOrArraySize	= 1;
	desc.MipLevels			= 1;
	desc.Format				= DXGI_FORMAT_UNKNOWN;
	desc.SampleDesc.Count	= 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout				= D3D12_TEXTURE_LAYOUT_ROW_MAJOR; //! �s�D��ŕۑ����ꂽ�e�N�X�`���f�[�^
	desc.Flags				= D3D12_RESOURCE_FLAG_NONE;

	// ���\�[�X�̐���
	auto hr = pDevice->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_pIB.GetAddressOf()));
	if (FAILED(hr))
		return false;


	// �������f�[�^������΁C��������ł���.
	if (pInitData != nullptr)
	{
		void* ptr = Map();
		if (ptr == nullptr)
			return false;

		memcpy(ptr, pInitData, count);

		m_pIB->Unmap(0, nullptr);
	}

	// �C���f�b�N�X�o�b�t�@�r���[�̐ݒ�
	m_View.BufferLocation = m_pIB->GetGPUVirtualAddress();
	m_View.Format = DXGI_FORMAT_R32_UINT;
	m_View.SizeInBytes = static_cast<UINT>(count);

	m_Count = count;

	return true;
}

//! @brief �C���f�b�N�X�o�b�t�@�̃}�b�s���O
void* IndexBuffer::Map() const
{
	void* ptr;
	auto hr = m_pIB->Map(0, nullptr, &ptr);
	if (FAILED(hr))
		return nullptr;

	return ptr;
}

//! @brief �C���f�b�N�X�o�b�t�@�̃A���}�b�v
void IndexBuffer::Unmap()
{
	m_pIB->Unmap(0, nullptr);
}

