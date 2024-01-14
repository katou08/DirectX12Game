#include "System/ImguiSystem.h"
#include "Game/Game.h"
#include "Game/Display.h"
#include "Window.h"
#include "Graphics/GraphicsEngine.h"
#include "Graphics/RenderSystem.h"
#include "Graphics/RenderContext.h"
#include "Graphics/ResourceHeap.h"

ImguiSystem::ImguiSystem(Game* game) : m_pGame(game)
{
}

ImguiSystem::~ImguiSystem()
{
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

bool ImguiSystem::Initialize()
{
	auto hwnd = static_cast<HWND>(m_pGame->GetDisplay()->GetWindow());
	m_pRenderSystem = m_pGame->GetGraphicsEngine()->GetRenderSystem();
	auto pDevice = m_pRenderSystem->GetDevice();

	// ディスクリプタ作成
	m_imguiHeap.Create(pDevice, "HeapForImgui", D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, 1);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX12_Init(pDevice, m_pRenderSystem->GetFrameCount(), m_pRenderSystem->GetBackBufferFormat(), m_imguiHeap.GetHeap(),
		m_imguiHeap.GetHeap()->GetCPUDescriptorHandleForHeapStart(), m_imguiHeap.GetHeap()->GetGPUDescriptorHandleForHeapStart());

	return true;
}

void ImguiSystem::Update()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	auto framerate = ImGui::GetIO().Framerate;
	ImGui::Begin("Control");
	ImGui::Text("Framerate(avg) %.3f ms/frame", 1000.0f / framerate);
	ImGui::Text("%.3f", text.x);
	ImGui::Text("%.3f", text.y);
	ImGui::Text("%.3f", text.z);
	ImGui::Text("%.3f", text1);
	ImGui::End();
}

void ImguiSystem::Render()
{
	// 描画前処理
	auto renderContext = m_pRenderSystem->GetRenderContext();
	renderContext->SetDescriptorHeap(m_imguiHeap.GetHeap());

	auto cmdList = m_pRenderSystem->GetRenderContext()->GetCommandList();
	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList);
}
