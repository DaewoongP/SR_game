#include "stdafx.h"
#include "ImguiMgr.h"
#include"imgui.h"
#include"imgui_impl_dx9.h"
#include"imgui_impl_win32.h"
#include"..\Engine\Export_Function.h"

#include "DefaultCube.h"
#include "InstallCube.h"

#include "ImguiStage.h"

IMPLEMENT_SINGLETON(CImguiMgr)

CImguiMgr::CImguiMgr()
	:m_pImguiStage(nullptr),
	m_bStageTool(true)
{
}

CImguiMgr::~CImguiMgr()
{
	Release();
}

HRESULT CImguiMgr::Ready_Imgui(LPDIRECT3DDEVICE9 m_pGraphicDev)
{
	m_pImguiStage = CImguiStage::Create(m_pGraphicDev);

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
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// //// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		// 큐브 설치 관련 변수
		static vector<CUBEINFO> vecCubeInfo; // 저장을 위한 큐브 벡터
		static CGameObject* pDefaultCube = nullptr; // 디폴트 큐브 형성
		static bool bCubePlaced = false; // 큐브 체크 박스
		static int iCubeIndex = 0;		 // 큐브 인덱스 번호
		static bool bCubeCreate = false;

		// 그리드 관련 변수

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
	if (nullptr != m_pInstance)
	{
		delete m_pImguiStage;
		m_pImguiStage = nullptr;
	}

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}