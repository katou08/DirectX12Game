#include "Graphics/SwapChain.h"
#include "Graphics/RenderSystem.h"

//! @brief �R���X�g���N�^
SwapChain::SwapChain()
{
}

//! @brief �f�X�g���N�^
SwapChain::~SwapChain()
{
}

//! @brief �X���b�v�`�F�[���̍쐬
//! @param[in] pDevice		�f�o�C�X
//! @param[in] width		����
//! @param[in] height		�c��
//! @param[in] m_renderer   �����_�[�V�X�e��
//! @return �X���b�v�`�F�[���̍쐬�ɐ���������
bool SwapChain::Initialize(HWND hwnd, UINT width, UINT height, RenderSystem* m_renderer)
{
	m_pRenderSystem = m_renderer;

	// �X���b�v�`�F�[���̐ݒ�
	DXGI_SWAP_CHAIN_DESC desc = {};
	desc.BufferDesc.Width					= (width > 1) ? width : 1;
	desc.BufferDesc.Height					= (height > 1) ? height : 1;
	desc.BufferDesc.RefreshRate.Numerator	= 60;
	desc.BufferDesc.RefreshRate.Denominator = 1;
	desc.BufferDesc.ScanlineOrdering		= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; //< �������`�惂�[�h�̐ݒ�
	desc.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count					= 1; //! �s�N�Z���P�ʂ̃}���`�T���v����
	desc.SampleDesc.Quality					= 0;
	desc.BufferUsage						= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.BufferCount						= m_renderer->frameCount;
	desc.OutputWindow						= hwnd;
	desc.Windowed							= TRUE;
	desc.SwapEffect							= DXGI_SWAP_EFFECT_FLIP_DISCARD; //! Present�̌�o�b�N�o�b�t�@�͔j��
	desc.Flags								= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //! �\�����[�h�؂�ւ��\

	// �X���b�v�`�F�C���̐���.
	ComPtr<IDXGISwapChain> pSwapChain = nullptr;
	auto hr = m_renderer->m_pFactory->CreateSwapChain(m_renderer->m_pCommandQueue.Get(), &desc, pSwapChain.GetAddressOf());
	ThrowIfFailed(hr, "�X���b�v�`�F�C���̐����Ɏ��s���܂���.");

	// IDXGISwapChain4���擾
	hr = pSwapChain.As(&m_pSwapChain);
	ThrowIfFailed(hr, "�X���b�v�`�F�C���̐����Ɏ��s���܂���.");

	// �s�v�ɂȂ����̂ŉ��.
	pSwapChain.Reset();

	return true;
}

//! @brief �v���[���g
//! @param[in] vsync �����������邩�ǂ���
void SwapChain::Present(UINT vsync)
{
	m_pSwapChain->Present(vsync, 0);
}

//! @brief ���T�C�Y
//! @param[in] width  ����
//! @param[in] height �c��
void SwapChain::Resize(UINT width, UINT height)
{
	m_bChangeScreenSize = true;
	// �o�b�N�o�b�t�@�����
	m_pRenderSystem->ReleaseBackBuffers();
	// �X���b�v�`�F�[���̎��o�b�t�@���T�C�Y�ύX
	m_pSwapChain->ResizeBuffers(0, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	// �T�C�Y�̕ύX�ɔ����CRTV,DSV�̍�蒼��
	m_pRenderSystem->CreateRenderTargetView();
	m_pRenderSystem->CreateDepthStencilView();
}

//! @brief ���݂̃o�b�N�o�b�t�@�̔ԍ����擾
UINT SwapChain::GetCurrentBackBufferIndex()
{
	return m_pSwapChain->GetCurrentBackBufferIndex();
}

//! @brief �o�b�N�o�b�t�@�̎擾
//! @param[in] i		�o�b�N�o�b�t�@�̔ԍ�
//! @param[in] pBuffer	�o�b�t�@���
HRESULT SwapChain::GetBuffer(UINT i, ID3D12Resource* pBuffer)
{
	auto hr = m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&pBuffer));

	return hr;
}
//! @brief �o�b�N�o�b�t�@�̐ݒ���̎擾
//! @param[in] pDesc	�f�X�N
HRESULT SwapChain::GetDesc(DXGI_SWAP_CHAIN_DESC* pDesc)
{
	auto hr = m_pSwapChain->GetDesc(pDesc);
	return hr;
}
