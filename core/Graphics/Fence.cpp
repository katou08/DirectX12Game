#include "Graphics/Fence.h"

//! @brief �R���X�g���N�^
//! @param[in] pDevice  �f�o�C�X
Fence::Fence(ID3D12Device* pDevice)
	:m_pFence(nullptr)
	,m_Event(nullptr)
	,m_Counter(0)
{
	if(pDevice == nullptr)
		ThrowFailed("�t�F���X�̍쐬�Ɏ��s���܂���.");

	// �C�x���g�̍쐬
	m_Event = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
	if(m_Event == nullptr)
		ThrowFailed("�t�F���X�̍쐬�Ɏ��s���܂���.");

	// �t�F���X�̍쐬
	auto hr = pDevice->CreateFence(
		0,
		D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(m_pFence.GetAddressOf()));
	ThrowIfFailed(hr, "�t�F���X�̍쐬�Ɏ��s���܂���.");

	// �J�E���^�[�ݒ�
	m_Counter = 1;
}

Fence::~Fence()
{
	// �n���h�������.
	if (m_Event != nullptr)
	{
		CloseHandle(m_Event);
		m_Event = nullptr;
	}

	// �t�F���X�I�u�W�F�N�g��j��.
	m_pFence.Reset();

	// �J�E���^�[���Z�b�g.
	m_Counter = 0;
}

//! @brief �V�O�i����ԂɂȂ�܂Ŏw�肳�ꂽ���ԑҋ@����
//! @param[in] pQueue  �R�}���h�L���[
//! @param[in] timeout �^�C���A�E�g����[ms]
void Fence::Wait(ID3D12CommandQueue* pQueue, uint32_t timeout)
{
	if (pQueue == nullptr)
		return;

	const auto fenceValue = m_Counter; //! ���݂̃t���[���̃t�F���X�J�E���^�[���擾

	// �V�O�i������
	auto hr = pQueue->Signal(m_pFence.Get(), fenceValue); //! �t�F���X�ɒl���Z�b�g
	if (FAILED(hr))
		return;

	// �J�E���^�[�𑝂₷
	m_Counter++;

	// ���̃t���[���`�揈�����܂��ł���Αҋ@
	if (m_pFence->GetCompletedValue() < fenceValue)
	{
		// �������ɃC�x���g��ݒ�
		auto hr = m_pFence->SetEventOnCompletion(fenceValue, m_Event);
		if (FAILED(hr))
			return;

		// �ҋ@����
		if (WAIT_OBJECT_0 != WaitForSingleObjectEx(m_Event, timeout, FALSE))
			return;
	}
}

//! @brief �V�O�i����ԂɂȂ�܂ł����Ƒҋ@����
//! @param[in] pQueue  �R�}���h�L���[
//! @param[in] timeout �^�C���A�E�g����[ms]
void Fence::Sync(ID3D12CommandQueue* pQueue)
{
	if (pQueue == nullptr)
		return;

	// �V�O�i������
	auto hr = pQueue->Signal(m_pFence.Get(), m_Counter);
	if (FAILED(hr))
		return;

	// �������ɃC�x���g��ݒ�
	hr = m_pFence->SetEventOnCompletion(m_Counter, m_Event);
	if (FAILED(hr))
		return;

	// �ҋ@����
	if (WAIT_OBJECT_0 != WaitForSingleObjectEx(m_Event, INFINITE, FALSE))
		return;

	// �J�E���^�[�𑝂₷
	m_Counter++;
}
