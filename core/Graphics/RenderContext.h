#pragma once
#include "pch.h"

class VertexBuffer;
class IndexBuffer;
class RenderContext
{
public:
	RenderContext();
	~RenderContext();
	bool Initialize(ID3D12Device* pDevice, D3D12_COMMAND_LIST_TYPE type, uint32_t count);
	void Term();
	void Close();
	ID3D12GraphicsCommandList* GetCommandList() const { return m_pCommandList.Get(); }
	ID3D12GraphicsCommandList* Reset();
	void SetViewportAndScissor(D3D12_VIEWPORT& viewport);
	void SetRootSignature(ID3D12RootSignature* rootsig);
	void SetPipelineState(ID3D12PipelineState* pso);
	void SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology);
	void SetResourceBarrier(ID3D12Resource* pResource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);
	void SetVertexBuffer(VertexBuffer& vb);
	void SetIndexBuffer(IndexBuffer& ib);
	void SetDescriptorHeap(ID3D12DescriptorHeap* pHeap);
	void SetGraphicsRootDescriptorTable(UINT RootParameterIndex, D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptor);
	void SetRenderTargets(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle);
	void ClearRenderTargetView(D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle, const float* clearColor);
	void ClearDepthStencilView(D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle, float clearValue);
	void DrawIndexed(UINT indexCount);
	void DrawIndexedInstanced(UINT indexCount, UINT instanceCount);
	void DrawVertices(UINT verticesCount);
private:
	void SetScissorRect(D3D12_RECT& rect);

	ComPtr<ID3D12GraphicsCommandList> m_pCommandList = nullptr;
	std::vector<ComPtr<ID3D12CommandAllocator>> m_pCommandAllocators = { nullptr }; //!< コマンドアロケータ
	uint32_t m_Index = 0; // アロケータのインデックス
	D3D12_VIEWPORT m_currentViewport = {};	// 現在のビューポート
};