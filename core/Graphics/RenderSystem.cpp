#include "pch.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/RenderContext.h"
#include "Game/Display.h"
#include "Graphics/SwapChain.h"
#include "Game/Game.h"

//! @brief �R���X�g���N�^
//! @param[in] game  �Q�[���N���X
RenderSystem::RenderSystem(Game* game) : m_pGame(game)
{
}

//! @brief �f�X�g���N�^
RenderSystem::~RenderSystem()
{
}

//! @brief ������
bool RenderSystem::Initialize()
{
	// �f�o�b�O�t���O�̒ǉ�
#if defined(DEBUG) || defined(_DEBUG)
	ComPtr<ID3D12Debug> debug;
	auto hr = D3D12GetDebugInterface(IID_PPV_ARGS(debug.GetAddressOf()));
	// �f�o�b�O���C���[��L����
	if (SUCCEEDED(hr))
		debug->EnableDebugLayer();
	else
		ThrowIfFailed(hr, "�f�o�b�O���C���[�̗L�����Ɏ��s���܂���.");
#endif

	if (!CreateFactory())
		return false;

	if (!CreateDevice())
		return false;

	if (!CreateRenderContext())
		return false;

	if (!CreateSwapChain())
		return false;

	if (!CreateDescriptorHeaps())
		return false;

	// RTV�̍쐬
	if (!CreateRenderTargetView())
		return false;

	// DSV�̍쐬
	if (!CreateDepthStencilView())
		return false;

	// �t�F���X�̍쐬
	m_pFence = std::make_unique<Fence>(m_pDevice.Get());
	if (m_pFence == nullptr)
		return false;

	// �r���[�|�[�g�̐ݒ�
	auto winSize = m_pGame->GetDisplay()->getClientSize();
	m_Viewport.Width = static_cast<FLOAT>(winSize.width);
	m_Viewport.Height = static_cast<FLOAT>(winSize.height);
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;

	return true;
}

//! @param[in] �`��O����
void RenderSystem::BeginRender()
{
	// ���݂̃t���[���C���f�b�N�X���擾
	m_FrameIndex = m_pGame->GetDisplay()->m_pSwapChain->GetCurrentBackBufferIndex();

	// �R�}���h���X�g,�R�}���h�A���P�[�^�[�����Z�b�g
	m_pRenderContext->Reset();

	// ���\�[�X�o���A�̃X�e�[�g�������_�[�^�[�Q�b�g�ɕύX
	m_pRenderContext->SetResourceBarrier(m_pBackBuffers[m_FrameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

	// �����_�[�^�[�Q�b�g���Z�b�g
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH = m_RTVs[m_FrameIndex].GetCPUDescHandle(); //!< RTV��CPU�n���h��
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = m_DSVs[0].GetCPUDescHandle(); //!< DSV��CPU�n���h��
	m_pRenderContext->SetRenderTargets(rtvH, dsvH);

	// �Z�b�g���������_�[�^�[�Q�b�g�̉�ʂ��N���A
	float clearColor[] = { 0.15f, 0.15f, 0.15f, 1.0f };
	m_pRenderContext->ClearRenderTargetView(rtvH, clearColor);

	// �[�x�X�e���V���r���[���N���A
	m_pRenderContext->ClearDepthStencilView(dsvH, 1.0f);

	// �r���[�|�[�g�̐ݒ�
	auto winSize = m_pGame->GetDisplay()->getClientSize();
	m_Viewport.Width = static_cast<FLOAT>(winSize.width);
	m_Viewport.Height = static_cast<FLOAT>(winSize.height);
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;

	m_pRenderContext->SetViewportAndScissor(m_Viewport);
}

void RenderSystem::Render()
{

}

//! @param[in] �`���̏���
void RenderSystem::EndRender()
{
	// ���\�[�X�o���A�̃X�e�[�g���v���[���g�ɕύX
	m_pRenderContext->SetResourceBarrier(m_pBackBuffers[m_FrameIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);

	//�R�}���h���X�g�����
	m_pRenderContext->Close();

	// �R�}���h�����s
	auto pCmd = m_pRenderContext->GetCommandList();
	ID3D12CommandList* pCmdLists[] = { pCmd };
	m_pCommandQueue->ExecuteCommandLists(1, pCmdLists);

	// ��ʕ\��
	Present(1);
}

void RenderSystem::WaitRender()
{
}

//! @param[in] �o�b�N�o�b�t�@�̃t�H�[�}�b�g���擾
const DXGI_FORMAT& RenderSystem::GetBackBufferFormat() const
{
	auto swapChain = m_pGame->GetDisplay()->m_pSwapChain->GetSwapChain();
	if (swapChain == nullptr)
		ThrowFailed("�X���b�v�`�F�[�����쐬����Ă��܂���.");

	DXGI_SWAP_CHAIN_DESC desc;
	swapChain->GetDesc(&desc);

	return desc.BufferDesc.Format;
}

//! @param[in] �[�x�o�b�t�@�̃t�H�[�}�b�g���擾
const DXGI_FORMAT& RenderSystem::GetDepthBufferFormat() const
{
	return mDepthBuffer.GetResource()->GetDesc().Format;
}

//! @param[in] �o�b�N�o�b�t�@�̉��
void RenderSystem::ReleaseBackBuffers()
{
	for (int i = 0; i < frameCount; ++i)
	{
		if (m_pBackBuffers[i])
			m_pBackBuffers[i]->Release();
	}
}

//! @brief �t�@�N�g���[�̐����֐�
bool RenderSystem::CreateFactory()
{
	UINT flagsDXGI = 0;
	flagsDXGI |= DXGI_CREATE_FACTORY_DEBUG; //!< DXGIDebug.dll������ꍇ�ǂݍ���
	auto hr = CreateDXGIFactory2(flagsDXGI, IID_PPV_ARGS(m_pFactory.GetAddressOf()));
	ThrowIfFailed(hr, "Factory�̍쐬�Ɏ��s���܂���.");

	return true;
}

//! @brief �f�o�C�X�����֐�
bool RenderSystem::CreateDevice()
{
	// �t�B�[�`���[���x�������v������̂�I��
	D3D_FEATURE_LEVEL levels[] = {
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
	};
	// �f�o�C�X�̐���
	for (auto lv : levels)
	{
		if (SUCCEEDED(D3D12CreateDevice(nullptr, lv, IID_PPV_ARGS(m_pDevice.GetAddressOf()))))
			break;
	}
	if (!m_pDevice)
	{
		ThrowFailed("Device�̍쐬�Ɏ��s���܂���.");
		return false;
	}

	return true;
}

//! @brief �����_�[�R���e�L�X�g�����֐�
bool RenderSystem::CreateRenderContext()
{
	D3D12_COMMAND_QUEUE_DESC desc = {};
	desc.Type		= D3D12_COMMAND_LIST_TYPE_DIRECT; //! GPU�����s�\�ȃR�}���h�o�b�t�@���w��
	desc.Priority	= D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
	desc.Flags		= D3D12_COMMAND_QUEUE_FLAG_NONE;
	desc.NodeMask	= 0; //! GPU����Ȃ�0

	auto hr = m_pDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(m_pCommandQueue.GetAddressOf()));
	ThrowIfFailed(hr, "�R�}���h�L���[�̍쐬�Ɏ��s���܂���.");

	m_pRenderContext = std::make_shared<RenderContext>();
	if(!m_pRenderContext->Initialize(m_pDevice.Get(), desc.Type, frameCount))
		ThrowFailed("�����_�[�R���e�L�X�g�̍쐬�Ɏ��s���܂���.");

	return true;
}

//! @brief �X���b�v�`�F�[���̍쐬
bool RenderSystem::CreateSwapChain()
{
	auto display = m_pGame->GetDisplay();
	if (!display->CreateSwapChain())
	{
		ThrowFailed("�X���b�v�`�F�[���̍쐬�Ɏ��s���܂���");
		return false;
	}

	return true;
}

//! @brief �e���\�[�X�̃f�B�X�N���v�^�q�[�v�����֐�
bool RenderSystem::CreateDescriptorHeaps()
{
	constexpr uint32_t numDescRTV = 100;
	constexpr uint32_t numDescDSV = 100;
	constexpr bool   bCPUVisible = false;
	
	// �q�[�v�̍쐬
	m_heapRTV.Create(m_pDevice.Get(), "HeapRTV", D3D12_DESCRIPTOR_HEAP_TYPE_RTV, numDescRTV);
	m_heapDSV.Create(m_pDevice.Get(), "HeapDSV", D3D12_DESCRIPTOR_HEAP_TYPE_DSV, numDescDSV);

	return true;
}

//! @brief RTV�̍쐬
bool RenderSystem::CreateRenderTargetView()
{
	m_pBackBuffers.resize(frameCount);
	auto swapChain = m_pGame->GetDisplay()->m_pSwapChain->GetSwapChain();
	if (swapChain == nullptr)
		ThrowFailed("�X���b�v�`�F�[�����쐬����Ă��܂���.");

	for (uint32_t i = 0u; i < frameCount; ++i)
	{
		auto hr = swapChain->GetBuffer(i, IID_PPV_ARGS(m_pBackBuffers[i].GetAddressOf()));
		ThrowIfFailed(hr, "RTV�p�̃o�b�t�@�̍쐬�Ɏ��s���܂���.");

		if (m_pBackBuffers[i] == nullptr)
			ThrowFailed("�o�b�N�o�b�t�@������܂���");

		DXGI_SWAP_CHAIN_DESC desc;
		swapChain->GetDesc(&desc);

		DXGI_FORMAT format = desc.BufferDesc.Format;

		D3D12_RENDER_TARGET_VIEW_DESC viewDesc = {};
		viewDesc.Format				  = format;
		viewDesc.ViewDimension		  = D3D12_RTV_DIMENSION_TEXTURE2D;
		viewDesc.Texture2D.MipSlice   = 0;
		viewDesc.Texture2D.PlaneSlice = 0;

		if (m_pGame->GetDisplay()->m_pSwapChain->GetIsChangeScreenSize() == false)
		{
			RenderTargetView rtv = {};
			m_heapRTV.AllocDescriptor(1, &rtv);
			m_RTVs.push_back(rtv);
		}

		m_pDevice->CreateRenderTargetView(m_pBackBuffers[i].Get(), &viewDesc, m_RTVs[i].GetCPUDescHandle());
	}

	return true;
}

//! @brief DSV�̍쐬
bool RenderSystem::CreateDepthStencilView()
{
	auto width = m_pGame->GetDisplay()->getClientSize().width;
	auto height = m_pGame->GetDisplay()->getClientSize().height;
	DXGI_FORMAT format = DXGI_FORMAT_D32_FLOAT;

	if (mDepthBuffer.GetResource())
		mDepthBuffer.Release();

	if (!mDepthBuffer.Create(m_pDevice.Get(), width, height, format, 1.0f, 0))
		ThrowFailed("�f�v�X�o�b�t�@�̍쐬�Ɏ��s���܂���.");

	// DSV�̍\���ݒ�
	D3D12_DEPTH_STENCIL_VIEW_DESC viewDesc = {};
	viewDesc.ViewDimension		 = D3D12_DSV_DIMENSION_TEXTURE2D;
	viewDesc.Format				 = format;
	viewDesc.Texture2D.MipSlice  = 0;
	viewDesc.Flags				 = D3D12_DSV_FLAG_NONE;

	if (m_pGame->GetDisplay()->m_pSwapChain->GetIsChangeScreenSize() == false)
	{
		// DSV�p�̃f�B�X�N���v�^�q�[�v�Ɋ��蓖�Ă�
		DepthStencilView dsv = {};
		m_heapDSV.AllocDescriptor(1, &dsv);
		m_DSVs.push_back(dsv);
	}

	// DSV�̐���
	m_pDevice->CreateDepthStencilView(mDepthBuffer.GetResource(), &viewDesc, m_DSVs[0].GetCPUDescHandle());
	return true;
}

//! @brief Present
//! //! @param[in] interval �����������邩�ǂ���
void RenderSystem::Present(uint32_t interval)
{
	// �X���b�v�`�F�[���Ƀv���[���g
	m_pGame->GetDisplay()->m_pSwapChain->Present(interval);
	// �����҂�
	m_pFence->Wait(m_pCommandQueue.Get(), INFINITE);
}
