#include "Graphics/RenderContext.h"
#include "Graphics/VertexBuffer.h"
#include "Graphics/IndexBuffer.h"

//! @brief �R���X�g���N�^
//! @param[in] game  �Q�[���N���X
RenderContext::RenderContext()
{
}

//! @brief �f�X�g���N�^
RenderContext::~RenderContext()
{
	Term();
}

//! @brief �������֐�
//! @param[in] pDevice  �f�o�C�X
//! @param[in] type		�R�}���h���X�g�̃^�C�v
//! @param[in] count	�R�}���h�A���P�[�^�̐�
bool RenderContext::Initialize(ID3D12Device* pDevice, D3D12_COMMAND_LIST_TYPE type, uint32_t count)
{
	if (pDevice == nullptr || count == 0)
		return false;

	m_pCommandAllocators.resize(count); // �A���P�[�^�̐������T�C�Y
	// �R�}���h�A���P�[�^�̐���(�R�}���h���X�g���g�p���郁�����̊��蓖�ėp)
	for (auto i = 0u; i < count; ++i)
	{
		auto hr = pDevice->CreateCommandAllocator(
			type, IID_PPV_ARGS(m_pCommandAllocators[i].GetAddressOf()));
		ThrowIfFailed(hr, "CommandAllocator�̍쐬�Ɏ��s���܂���.");
	}

	// �R�}���h���X�g�̐���
	auto hr = pDevice->CreateCommandList(
		0,
		type,
		m_pCommandAllocators[0].Get(), //! �o�b�N�o�b�t�@�ɑΉ�����A���P�[�^
		nullptr,
		IID_PPV_ARGS(m_pCommandList.GetAddressOf()));
	ThrowIfFailed(hr, "CommandList�̍쐬�Ɏ��s���܂���.");

	m_pCommandList->Close();

	m_Index = 0;

	return true;
}

//! @brief �R�}���h���X�g�C�A���P�[�^�I������
void RenderContext::Term()
{
	m_pCommandList.Reset();

	for (size_t i = 0; i < m_pCommandAllocators.size(); ++i)
	{
		m_pCommandAllocators[i].Reset();
	}

	m_pCommandAllocators.clear();
	m_pCommandAllocators.shrink_to_fit();
}

//! @brief �R�}���h���X�g�����
void RenderContext::Close()
{
	m_pCommandList->Close();
}

//! @brief  ���Z�b�g�������s�����R�}���h���X�g���擾
//! @return ���Z�b�g�������s�����R�}���h���X�g��Ԃ�
ID3D12GraphicsCommandList* RenderContext::Reset()
{
	auto hr = m_pCommandAllocators[m_Index]->Reset();
	if (FAILED(hr))
		return nullptr;

	hr = m_pCommandList->Reset(m_pCommandAllocators[m_Index].Get(), nullptr);
	if (FAILED(hr))
		return nullptr;

	m_Index = (m_Index + 1) % static_cast<uint32_t>(m_pCommandAllocators.size());

	return m_pCommandList.Get();
}

//! @brief �V�U�[��`�̐ݒ�ƃr���[�|�[�g�̃Z�b�g
//! @param[in] viewport �r���[�|�[�g
void RenderContext::SetViewportAndScissor(D3D12_VIEWPORT& viewport)
{
	//�V�U�����O��`���ݒ肷��B
	D3D12_RECT scissorRect;
	scissorRect.bottom = static_cast<LONG>(viewport.Height);
	scissorRect.top = 0;
	scissorRect.left = 0;
	scissorRect.right = static_cast<LONG>(viewport.Width);
	SetScissorRect(scissorRect);

	m_pCommandList->RSSetViewports(1, &viewport);
	m_currentViewport = viewport;
}

//! @brief �`�揈��
//! @param[in] viewport �r���[�|�[�g
//! @param[in] indexCount �C���f�b�N�X��
void RenderContext::DrawIndexed(UINT indexCount)
{
	m_pCommandList->DrawIndexedInstanced(indexCount, 1, 0, 0, 0);
}

//! @brief �`�揈�� (�C���X�^���V���O�`��)
//! //! @param[in] indexCount �C���f�b�N�X��
//! //! @param[in] instanceCount �C���X�^���X��
void RenderContext::DrawIndexedInstanced(UINT indexCount, UINT instanceCount)
{
	m_pCommandList->DrawIndexedInstanced(indexCount, instanceCount, 0, 0, 0);
}

//! @brief �`�揈�� (���_�`��)
//! @param[in] verticesCount ���_��
void RenderContext::DrawVertices(UINT verticesCount)
{
	m_pCommandList->DrawInstanced(verticesCount, 1, 0, 0);
}

//! @brief �V�U�[��`�̃Z�b�g
//! @param[in] rect Rect
void RenderContext::SetScissorRect(D3D12_RECT& rect)
{
	m_pCommandList->RSSetScissorRects(1, &rect);
}

//! @brief ���[�g�V�O�l�`���̃Z�b�g
//! @param[in] rootsig ���[�g�V�O�l�`��
void RenderContext::SetRootSignature(ID3D12RootSignature* rootsig)
{
	m_pCommandList->SetGraphicsRootSignature(rootsig);
}

//! @brief �p�C�v���C���X�e�[�g�̃Z�b�g
//! @param[in] pso �p�C�v���C���X�e�[�g
void RenderContext::SetPipelineState(ID3D12PipelineState* pso)
{
	m_pCommandList->SetPipelineState(pso);
}

//! @brief �v���~�e�B�u�g�|���W�[�̃Z�b�g
//! @param[in] topology �g�|���W�[
void RenderContext::SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology)
{
	m_pCommandList->IASetPrimitiveTopology(topology);
}

//! @brief ���\�[�X�o���A
//! @param[in] commandList	�R�}���h���X�g
//! @param[in] pResource	���\�[�X
//! @param[in] before		�O�̃��\�[�X�̏��
//! @param[in] after		���̃��\�[�X�̏��
void RenderContext::SetResourceBarrier(ID3D12Resource* pResource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
{
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Transition.pResource = pResource;
	barrier.Transition.StateBefore = before;
	barrier.Transition.StateAfter = after;
	m_pCommandList->ResourceBarrier(1, &barrier);
}

//! @brief ���_�o�b�t�@�̃Z�b�g
//! @param[in] vb	���_�o�b�t�@
void RenderContext::SetVertexBuffer(VertexBuffer& vb)
{
	auto VBV = vb.GetView();
	m_pCommandList->IASetVertexBuffers(0, 1, &VBV);
}

//! @brief �C���f�b�N�X�o�b�t�@�̃Z�b�g
//! @param[in] ib	�C���f�b�N�X�o�b�t�@
void RenderContext::SetIndexBuffer(IndexBuffer& ib)
{
	auto IBV = ib.GetView();
	m_pCommandList->IASetIndexBuffer(&IBV);
}

//! @brief �f�B�X�N���v�^�q�[�v�̃Z�b�g
//! @param[in] pHeap	�f�B�X�N���v�^�q�[�v
void RenderContext::SetDescriptorHeap(ID3D12DescriptorHeap* pHeap)
{
	ID3D12DescriptorHeap* pDescHeaps[] = { pHeap };
	m_pCommandList->SetDescriptorHeaps(_countof(pDescHeaps), pDescHeaps);
}

//! @brief �f�B�X�N���v�^�e�[�u���̃Z�b�g
//! @param[in] RootParameterIndex ���[�g�p�����^�̔ԍ�
//! @param[in] BaseDescriptor �f�B�X�N���v�^�̃x�[�XGPU�n���h��
void RenderContext::SetGraphicsRootDescriptorTable(UINT RootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor)
{
	m_pCommandList->SetGraphicsRootDescriptorTable(
		RootParameterIndex,
		BaseDescriptor
	);
}

//! @brief �����_�[�^�[�Q�b�g�̃Z�b�g
//! @param[in] rtvHandle RTV��CPU�n���h��
//! @param[in] dsvHandle DSV��CPU�n���h��
void RenderContext::SetRenderTargets(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle)
{
	m_pCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);
}

//! @brief �����_�[�^�[�Q�b�g�r���[�̃N���A
//! @param[in] rtvHandle RTV��CPU�n���h��
//! @param[in] clearColor �N���A�l
void RenderContext::ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, const float* clearColor)
{
	m_pCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
}

//! @brief �[�x�o�b�t�@�r���[�̃N���A
//! @param[in] dsvHandle DSV��CPU�n���h��
//! @param[in] clearColor �N���A�l
void RenderContext::ClearDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle, float clearValue)
{
	m_pCommandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
		clearValue, 0, 0, nullptr);
}
