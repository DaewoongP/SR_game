#include "stdafx.h"
#include "ImguiMgr.h"
#include"imgui.h"
#include"imgui_impl_dx9.h"
#include"imgui_impl_win32.h"
#include"..\Engine\Export_Function.h"

#include "ImguiStage.h"
#include "ImguiUnit.h"
#include"ImguiBG.h"

IMPLEMENT_SINGLETON(CImguiMgr)

CImguiMgr::CImguiMgr()
	:m_pImguiStage(nullptr), m_pImguiUnit(nullptr), m_pImguiBG(nullptr), m_pToodee(nullptr),
	m_bStageTool(false), m_bUnitTool(false), m_bOnceLoad(true), m_bDeleteAll(false),
	m_iStageNumber(0)
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
	m_pImguiBG =   CImguiBG::Create(m_pGraphicDev);

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
	bool show_demo_window = true;

	//// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).

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

		{
			ImGui::Checkbox("BackGround", &m_bBGTool);

			if (m_bBGTool)
			{
				ImGui::Begin("BackGround");

				m_pImguiBG->Update_Imgui_Unit();

				ImGui::End();

			}

		}

		// 스테이지 선택 콤보 박스
		const char* items[] = { "1", "2", "3", "4(boss2)", "5", "6", "7", "8(boss3)", "9", "10", 
								"Final1", "Final2", "99", "Mini_13", "Mini_14" };
		ImGui::Combo("StageNumber", &m_iStageNumber, items, IM_ARRAYSIZE(items));
		m_pImguiStage->Set_StageNumber(m_iStageNumber);
		m_pImguiUnit->Set_StageNumber(m_iStageNumber);
		m_pImguiBG->Set_StageNumber(m_iStageNumber);
		// 전부 세이브 버튼
		ImGui::Text("All Save and Load");
		
		if (ImGui::Button("SAVE ALL"))
		{
			m_pImguiStage->SaveCube(m_iStageNumber);
			m_pImguiStage->SaveGrid(m_iStageNumber);
			m_pImguiUnit->SaveMapObject(m_iStageNumber);
			m_pImguiUnit->SaveMonster(m_iStageNumber);
			m_pImguiBG->SaveBG(m_iStageNumber);
		}

		// 전부 로드 버튼
		ImGui::SameLine();
		if (ImGui::Button("LOAD ALL"))
		{
			m_pImguiStage->LoadCube(m_iStageNumber);
			m_pImguiStage->LoadGrid(m_iStageNumber);
			m_pImguiUnit->LoadMapObject(m_iStageNumber);
			m_pImguiUnit->LoadMonster(m_iStageNumber);
			m_pImguiBG->LoadBG(m_iStageNumber);
		}
		// 레이어의 모든 요소 지우기(맵 큐브 제외)
		ImGui::Text("All Delete");
		if (ImGui::Button("DELETE ALL"))
			m_bDeleteAll = true;

		// 선택지, 진짜 다 지울꺼냐 말꺼냐
		if (m_bDeleteAll)
		{
			ImGui::SameLine();
			ImGui::Text("Are you sure?");

			ImGui::SameLine();
			if (ImGui::Button("Yes"))
			{
				CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
				pLayer->Delete_In_Layer();
				pLayer = Engine::Get_Layer(L"Layer_Environment");
				pLayer->Delete_In_Layer();
				m_pImguiUnit->Get_LaserVector()->clear();
				m_pImguiUnit->Get_MonsterVector()->clear();
				m_pImguiUnit->Get_MapVector()->clear();
				m_pImguiUnit->Get_PortalVector()->clear();
				m_pImguiStage->Get_CubeVector()->clear();
				m_pImguiStage->Get_GridVector()->clear();
				

				m_bDeleteAll = false;
			}

			ImGui::SameLine();
			if (ImGui::Button("No"))
				m_bDeleteAll = false;
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
	m_pImguiBG->Release();

	Safe_Delete(m_pImguiStage);
	Safe_Delete(m_pImguiUnit);
	Safe_Delete(m_pImguiBG);

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}