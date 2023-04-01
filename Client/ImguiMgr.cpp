#include "stdafx.h"
#include "ImguiMgr.h"
#include"imgui.h"
#include"imgui_impl_dx9.h"
#include"imgui_impl_win32.h"
#include"..\Engine\Export_Function.h"

#include "Cube.h"

CImguiMgr::CImguiMgr()
{

}

CImguiMgr::~CImguiMgr()
{
	
}

HRESULT CImguiMgr::Update_Imgui(LPDIRECT3DDEVICE9 m_pGraphicDev)
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	// 플레이어 값 변경 스태틱 변수
	static int e = 0;
	static bool bReset = false;

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	static bool show_demo_window = false;
	static bool show_another_window = false;
	//// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// //// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{		
		//static float f = 0.0f;
		static int counter = 0;
		ImGui::Begin("Hello,Imgui!");                          // Create a window called "Hello, world!" and append into it.

		//ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		//ImGui::Checkbox("Another Window", &show_another_window);

		//ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f		

#pragma region imgui 연습 부분, 커밋할 떄 뺄 것
		//// 플레이어 리셋
		//ImGui::Checkbox("Reset", &bReset);

		//// 플레이어 값 변경 라디오 버튼 3개
		//ImGui::RadioButton("Scale", &e, 0); ImGui::SameLine();
		//ImGui::RadioButton("Rotation", &e, 1); ImGui::SameLine();
		//ImGui::RadioButton("Translation", &e, 2);

		//// 플레이어 값
		//ImGui::SliderFloat("X", &fX, -1.f, 100.0f);
		//ImGui::SliderFloat("Y", &fY, -1.f, 100.0f);
		//ImGui::SliderFloat("Z", &fZ, -1.f, 100.0f);

		//if (0 == e) // 스케일
		//	pPlayerTransform->m_vScale = { fX + 1.f, fY + 1.f, fZ + 1.f };

		//else if (1 == e) // 로테이션
		//{
		//	pPlayerTransform->Rotation(ROT_X, D3DXToRadian(fX));
		//	pPlayerTransform->Rotation(ROT_Y, D3DXToRadian(fY));
		//	pPlayerTransform->Rotation(ROT_Z, D3DXToRadian(fZ));
		//}

		//else if (2 == e) // 이동(원점 기준)
		//	pPlayerTransform->Set_Pos(fX, fY, fZ);

		//if (bReset)
		//{
		//	pPlayerTransform->m_vScale = { 1.f, 1.f, 1.f };
		//	pPlayerTransform->Set_Pos(1.f, 1.f, 1.f);
		//}
#pragma endregion

		const char* items[] = { "6_Cube", "4_Cube" };
		static int item_current = 0;
		ImGui::Combo("combo", &item_current, items, IM_ARRAYSIZE(items));
		//pPlantCube->Set_CubeIndex(item_current);

		static bool bCubePlaced = false;
		static int iCubeIndex = 0;
		ImGui::Checkbox("Cube Placed", &bCubePlaced);

		if (bCubePlaced)
		{
			CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			NULL_CHECK_RETURN(pStageLayer, E_FAIL);

			CGameObject* pGameObject = nullptr;

			if (GetAsyncKeyState('Q'))
			{
				_tchar strCubeIndex[64] = { 0 };
				_stprintf_s(strCubeIndex, _T("CubeIndex%d"), iCubeIndex);
				pGameObject = CCube::Create(m_pGraphicDev);
				pGameObject->m_pTransform->m_vInfo[INFO_POS] = _vec3{10.f, 7.f, 10.f};
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				FAILED_CHECK_RETURN(pStageLayer->Add_GameObject(strCubeIndex, pGameObject), E_FAIL);
				++iCubeIndex;
			}
		}

		//ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		//if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		//	counter++;
		//ImGui::SameLine();
		//ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
	}

	//// 3. Show another simple window.
	if (show_another_window)
	{
		ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
		ImGui::Text("Hello from another window!");
		if (ImGui::Button("Close Me"))
			show_another_window = false;
		ImGui::End();
	}

	// Rendering
	ImGui::EndFrame();
	ImGui::Render();

	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
	m_pGraphicDev->EndScene();
	m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
	return S_OK;
}

void CImguiMgr::Release()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
