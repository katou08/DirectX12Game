#include "Graphics/VertexBuffer.h"

//! @brief �R���X�g���N�^
VertexBuffer::VertexBuffer()
	:m_pVB(nullptr)
{
	memset(&m_View, 0, sizeof(m_View));
}

//! @brief �f�X�g���N�^
VertexBuffer::~VertexBuffer()
{
	m_pVB.Reset();
	memset(&m_View, 0, sizeof(m_View));
}

//! @brief ���_�o�b�t�@�̍쐬
//! @param[in] pDevice		�f�o�C�X
//! @param[in] size			�o�b�t�@�̑傫��
//! @param[in] stride		�X�g���C�h
//! @param[in] pInitData	�}�b�s���O����o�b�t�@�̃|�C���^
//! @return ���_�o�b�t�@�̍쐬�ɐ���������
bool VertexBuffer::Init(ID3D12Device* pDevice, size_t size, size_t stride, const void* pInitData)
{
	// �����`�F�b�N.
	if (pDevice == nullptr || size == 0 || stride == 0)
		return false;

	// �q�[�v�v���p�e�B�̍쐬
	D3D12_HEAP_PROPERTIES prop = {};
	prop.Type					= D3D12_HEAP_TYPE_UPLOAD; // GPU�ɓ]�����邽�߂̃q�[�v
	prop.CPUPageProperty		= D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference	= D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask		= 1; //! �P���GPU��O��̂���1
	prop.VisibleNodeMask		= 1;

	// ���\�[�X�̐ݒ�
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension			= D3D12_RESOURCE_DIMENSION_BUFFER; // �o�b�t�@�Ƃ��Ďg�p
	resDesc.Alignment			= 0;
	resDesc.Width				= UINT64(size);
	resDesc.Height				= 1;
	resDesc.DepthOrArraySize	= 1;
	resDesc.MipLevels			= 1;
	resDesc.Format				= DXGI_FORMAT_UNKNOWN;
	resDesc.SampleDesc.Count	= 1;
	resDesc.SampleDesc.Quality	= 0;
	resDesc.Layout				= D3D12_TEXTURE_LAYOUT_ROW_MAJOR; //! �s�D��ŕۑ����ꂽ�f�[�^
	resDesc.Flags				= D3D12_RESOURCE_FLAG_NONE;

	// ���\�[�X�̍쐬
	auto hr = pDevice->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_pVB.GetAddressOf()));
	if (FAILED(hr))
		return false;

	// �}�b�s���O
	if (pInitData != nullptr)
	{
		// �}�b�s���O(���_�f�[�^���e�����\�[�X�ɏ�������)
		void* ptr = Map();
		if (ptr == nullptr)
			return false;

		// ���_�f�[�^���}�b�s���O��ɐݒ�
		memcpy(ptr, pInitData, size);

		m_pVB->Unmap(0, nullptr); //!< �}�b�s���O����

	}

	// ���_�o�b�t�@�r���[�̐ݒ�
	m_View.BufferLocation = m_pVB->GetGPUVirtualAddress();
	m_View.SizeInBytes = static_cast<UINT>(size);
	m_View.StrideInBytes = static_cast<UINT>(stride);

	return true;
}

void* VertexBuffer::Map() const
{
	void* ptr;
	auto hr = m_pVB->Map(0, nullptr, &ptr);
	if (FAILED(hr))
		return nullptr;

	return ptr;
}

void VertexBuffer::Unmap()
{
	m_pVB->Unmap(0, nullptr);
}
