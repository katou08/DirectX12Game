#include "DepthBuffer.h"

//! @brief DepthBuffer�̍쐬
//! @param[in] pDevice  �f�o�C�X
//! @param[in] width	�o�b�t�@�̕�
//! @param[in] height	�o�b�t�@�̍���
//! @param[in] format	�o�b�t�@�̃t�H�[�}�b�g
//! @param[in] clearDepth  �N���A�l
//! @param[in] clearStencil  �[�x�X�e���V���l
//! @return �f�v�X�o�b�t�@�̐����ɐ���������
bool DepthBuffer::Create(ID3D12Device* pDevice, uint32_t width, uint32_t height, DXGI_FORMAT format, float clearDepth, uint8_t clearStencil)
{
	if (pDevice == nullptr)
		return false;

	// �q�[�v�̐ݒ�
	D3D12_HEAP_PROPERTIES prop = {};
	prop.Type					= D3D12_HEAP_TYPE_DEFAULT;
	prop.CPUPageProperty		= D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference	= D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask		= 1;
	prop.VisibleNodeMask		= 1;

	// ���\�[�X�̐ݒ�
	D3D12_RESOURCE_DESC desc = {};
	desc.Dimension			= D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc.Alignment			= 0;
	desc.Width				= static_cast<UINT64>(width);
	desc.Height				= height;
	desc.DepthOrArraySize	= 1;
	desc.MipLevels			= 1;
	desc.Format				= static_cast<DXGI_FORMAT>(format);
	desc.SampleDesc.Count	= 1;
	desc.SampleDesc.Quality = 0;
	desc.Layout				= D3D12_TEXTURE_LAYOUT_UNKNOWN;
	desc.Flags				= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	// �f�v�X�o�b�t�@�̃t�H�[�}�b�g�ƍő�[�x�l��ݒ�
	D3D12_CLEAR_VALUE depthClearValue = {};
	depthClearValue.DepthStencil.Depth	 = 1.0f;
	depthClearValue.DepthStencil.Stencil = 0;
	depthClearValue.Format				 = static_cast<DXGI_FORMAT>(format);

	// �f�v�X�o�b�t�@�̐���
	auto hr = pDevice->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(m_pBuffer.GetAddressOf()));
	ThrowIfFailed(hr, "�f�v�X�o�b�t�@�̍쐬�Ɏ��s���܂���.");

	return true;
}

void DepthBuffer::Release()
{
	if (m_pBuffer)
		m_pBuffer->Release();
}
