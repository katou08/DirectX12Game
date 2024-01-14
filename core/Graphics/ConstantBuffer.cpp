#include "Graphics/ConstantBuffer.h"

ConstantBuffer::ConstantBuffer()
{

}

ConstantBuffer::~ConstantBuffer()
{

}

//! @brief CBの作成
//! @param[in] pDevice  デバイス
//! @param[in] size		データのサイズ
//! @param[in] name		CBの名前
//! @param[in] srcData  マッピングするバッファのポインタ
//! @return CBの作成に成功したか
bool ConstantBuffer::Init(ID3D12Device* pDevice, uint32_t size, const std::string& name, void* srcData)
{
	if (pDevice == nullptr || size == 0)
		return false;

	// ヒーププロパティ
	D3D12_HEAP_PROPERTIES prop = {};
	prop.Type				  = D3D12_HEAP_TYPE_UPLOAD;
	prop.CPUPageProperty	  = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	prop.CreationNodeMask	  = 1;
	prop.VisibleNodeMask	  = 1;

	//定数バッファは256バイトアライメントが要求されるので、256の倍数に切り上げる
	m_allocSize = (size + 256) & 0xFFFFFF00;

	// リソースの設定
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

	// 定数バッファの作成
	auto hr = pDevice->CreateCommittedResource(
		&prop,
		D3D12_HEAP_FLAG_NONE,
		&resDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(m_pCB.GetAddressOf()));
	ThrowIfFailed(hr, "コンスタントバッファの作成に失敗しました.");
	m_pCB->SetName(Utility::StringToWString(name).c_str());

	//定数バッファをCPUからアクセス可能な仮想アドレス空間にマッピングする
	D3D12_RANGE	readRange = { 0, 0 };		// CPUからバッファを読み込まない設定
	hr = m_pCB->Map(0, &readRange, &m_pMappedPtr);
	if (FAILED(hr))
		return false;

	m_Desc.BufferLocation = m_pCB->GetGPUVirtualAddress();
	m_Desc.SizeInBytes	  = static_cast<UINT>(m_allocSize);

	return true;
}
