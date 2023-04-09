#include "stdafx.h"
#include "ImguiMgr.h"
#include"imgui.h"
#include"imgui_impl_dx9.h"
#include"imgui_impl_win32.h"
#include"..\Engine\Export_Function.h"

#include "ImguiStage.h"
#include "ImguiUnit.h"

IMPLEMENT_SINGLETON(CImguiMgr)

CImguiMgr::CImguiMgr()
	:m_pImguiStage(nullptr), m_pImguiUnit(nullptr), m_pToodee(nullptr),
	m_bStageTool(false), m_bUnitTool(false), m_bOnceLoad(true)
{
}

CImguiMgr::~CImguiMgr()
{
	Release();
}

HRESULT CImguiMgr::Ready_Imgui(LPDIRECT3DDEVICE9 m_pGraphicDev)
{
	m_pImguiStage = CImguiStage::Create(m_pGraphicDev);
	m_pImguiUnit = CImguiUnit::Create(m_pGraphicDev);

	return S_OK;
}

HRESULT CImguiMgr::Update_Imgui(LPDIRECT3DDEVICE9 m_pGraphicDev)
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	bool show_demo_window = false;

	//// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// //// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		// 본문 시작
		ImGui::Begin("Hello,Imgui!");

		// 데모 툴 on / off(참고용)
		ImGui::Checkbox("Demo Window", &show_demo_window);	

		// 스테이지 툴 on / off
		{
			ImGui::Checkbox("StageTool", &m_bStageTool);

			if (m_bStageTool)
			{
				ImGui::Begin("StageTool");

				m_pImguiStage->Update_Imgui_Stage();

				ImGui::End();
			}
		}

		// 유닛 툴 on / off
		{
			ImGui::Checkbox("UnitTool", &m_bUnitTool);

			if (m_bUnitTool)
			{
				ImGui::Begin("UnitTool");

				m_pImguiUnit->Update_Imgui_Unit();

				ImGui::End();
			}
		}

		// 전부 세이브 버튼
		ImGui::Text("All Save and Load");
		
		if (ImGui::Button("SAVE ALL"))
		{
			m_pImguiStage->SaveCube();
			m_pImguiStage->SaveGrid();
			m_pImguiUnit->SaveMapObject();
			m_pImguiUnit->SaveMonster();
		}

		// 전부 로드 버튼
		ImGui::SameLine();
		if (ImGui::Button("LOAD ALL"))
		{
			m_pImguiStage->LoadCube();
			m_pImguiStage->LoadGrid();
			m_pImguiUnit->LoadMapObject();
			m_pImguiUnit->LoadMonster();
		}

		// 프로그램 시작하자 마자 로딩
		m_pToodee = Engine::Get_GameObject(L"Layer_GameLogic", L"Toodee");
		if (m_pToodee && m_bOnceLoad)
		{
			m_pImguiStage->LoadCube();
			m_pImguiStage->LoadGrid();
			m_pImguiUnit->LoadMapObject();
			m_pImguiUnit->LoadMonster();
			m_bOnceLoad = false;
		}

		// 마우스 커서 위치
		if (ImGui::IsMousePosValid())
			ImGui::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);

		// 현재 프레임 정보
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

		ImGui::End();
	}

	// Rendering
	ImGui::EndFrame();
	ImGui::Render();

	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	m_pGraphicDev->EndScene();
	//m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
	return S_OK;
}

void CImguiMgr::Release()
{
	m_pImguiStage->Release();
	m_pImguiUnit->Release();

	Safe_Delete(m_pImguiStage);
	Safe_Delete(m_pImguiUnit);

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}