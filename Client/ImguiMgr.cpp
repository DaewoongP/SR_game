#include "stdafx.h"
#include "ImguiMgr.h"
#include"imgui.h"
#include"imgui_impl_dx9.h"
#include"imgui_impl_win32.h"
#include"..\Engine\Export_Function.h"
#include "Cube.h"
#include "Grid.h"

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

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	bool show_demo_window = true;

	//// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	// //// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{		
		// 세이브 로드 관련 변수
		static bool bSaveButton = false;
		static bool bLoadButton = false;

		// 큐브 설치 관련 변수
		static vector<_vec3> vecCubePos;
		static CGameObject* pDefaultCube = nullptr; // 디폴트 큐브 형성
		static bool bCubePlaced = false;
		static int iCubeIndex = 0;

		// 그리드 생성 관련 변수
		static bool bGridON = false;
		static bool bGridCreate = true;

		// 큐브 선택 관련 변수
		static int item_current = 0;

		ImGui::Begin("Hello,Imgui!");

		ImGui::Checkbox("Demo Window", &show_demo_window);

		// 콤보 박스로 큐브 텍스처 고르는 기능 아직 미구현
		const char* items[] = { "6_Cube", "4_Cube" };
		ImGui::Combo("Cube Index", &item_current, items, IM_ARRAYSIZE(items));
		//pPlantCube->Set_CubeIndex(item_current);

		ImGui::Checkbox("Grid", &bGridON); // 그리드 체크 박스

		if (bGridON) // 그리드 체크 박스 활성화 시 그리드 생성
		{
			if (bGridCreate)
			{
				CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
				NULL_CHECK_RETURN(pStageLayer, E_FAIL);
				CGameObject* pGameObject = nullptr;

				int iGridIndex = 0;
				for (int i = 0; i < CUBEY; ++i)
				{
					for (int j = 0; j < CUBEX; ++j)
					{
						iGridIndex = i * CUBEX + j;

						TCHAR objName[128] = { 0 };
						_stprintf_s(objName, _T("Grid_Tile%d"), (iGridIndex));
						pGameObject = CGrid::Create(m_pGraphicDev);
						pGameObject->m_pTransform->m_vInfo[INFO_POS] = _vec3{ (float)j*1.99f,(float)i*1.99f,10.f };
						NULL_CHECK_RETURN(pGameObject, E_FAIL);
						FAILED_CHECK_RETURN(pStageLayer->Add_GameObject(objName, pGameObject), E_FAIL);
					}
				}
				bGridCreate = false;
			}
		}

		
		ImGui::Checkbox("Cube Placed", &bCubePlaced); // 큐브 설치 체크 박스

		// 디폴트 큐브가 생성되어 있는데 체크 항목이 꺼질 경우 디폴트 큐브 사망처리
		if (!bCubePlaced && nullptr != pDefaultCube)
		{
			pDefaultCube->m_bDead = true;
			pDefaultCube = nullptr;
		}			

		// 체크 박스 활성회 시 큐브 설치 부분
		if (bCubePlaced)
		{
			CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			NULL_CHECK_RETURN(pStageLayer, E_FAIL);

			CGameObject* pGameObject = nullptr;

			if (nullptr == pDefaultCube)
			{
				pDefaultCube = CCube::Create(m_pGraphicDev);
				pDefaultCube->m_pTransform->m_vInfo[INFO_POS] = _vec3{ 10.f, 10.f, 10.f };
				pDefaultCube->m_pTransform->m_bIsStatic = true;
				NULL_CHECK_RETURN(pDefaultCube, E_FAIL);
				FAILED_CHECK_RETURN(pStageLayer->Add_GameObject(L"DefaultCube", pDefaultCube), E_FAIL);
			}

			// 디폴트 큐브의 움직임(대충)
			{
				if (Engine::Get_DIKeyState(DIK_A) == Engine::KEYDOWN) // 좌
					pDefaultCube->m_pTransform->m_vInfo[INFO_POS].x -= 2.f;

				if (Engine::Get_DIKeyState(DIK_D) == Engine::KEYDOWN) // 우
					pDefaultCube->m_pTransform->m_vInfo[INFO_POS].x += 2.f;

				if (Engine::Get_DIKeyState(DIK_W) == Engine::KEYDOWN) // 상
					pDefaultCube->m_pTransform->m_vInfo[INFO_POS].y += 2.f;

				if (Engine::Get_DIKeyState(DIK_S) == Engine::KEYDOWN) // 하
					pDefaultCube->m_pTransform->m_vInfo[INFO_POS].y -= 2.f;
			}

			if (Engine::Get_DIKeyState(DIK_E) == Engine::KEYDOWN)
			{
				_tchar strCubeIndex[64] = { 0 };
				_stprintf_s(strCubeIndex, _T("CubeIndex%d"), iCubeIndex);
				pGameObject = CCube::Create(m_pGraphicDev);
				pGameObject->m_pTransform->m_vInfo[INFO_POS] = pDefaultCube->m_pTransform->m_vInfo[INFO_POS];
				pGameObject->m_pTransform->m_bIsStatic = true;
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				FAILED_CHECK_RETURN(pStageLayer->Add_GameObject(strCubeIndex, pGameObject), E_FAIL);
				vecCubePos.push_back(pGameObject->m_pTransform->m_vInfo[INFO_POS]); // 저장을 위함
				++iCubeIndex;
			}
		}

		if (ImGui::IsMousePosValid())
			ImGui::Text("/ Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);		

		if (ImGui::Button("Cube Save")) // 저장 기능
		{
			bSaveButton = false;
			HANDLE hFile = CreateFile(L"../Data/CubePos.dat", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
			if (INVALID_HANDLE_VALUE == hFile)
				return E_FAIL;

			DWORD	dwByte = 0;

			for (auto& iter : vecCubePos)
				WriteFile(hFile, iter, sizeof(_vec3), &dwByte, nullptr);

			CloseHandle(hFile);
		}
		
		ImGui::SameLine();		
		if (ImGui::Button("Cube Load")) // 로드 기능
		{
			bLoadButton = false;
			vecCubePos.clear();

			HANDLE hFile = CreateFile(L"../Data/CubePos.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

			if (INVALID_HANDLE_VALUE == hFile)
				return E_FAIL;

			DWORD	dwByte = 0;
			_vec3 vCubePos = {0,0,0};

			CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			NULL_CHECK_RETURN(pStageLayer, E_FAIL);

			CGameObject* pGameObject = nullptr;

			while (true)
			{
				ReadFile(hFile, &vCubePos, sizeof(_vec3), &dwByte, nullptr);
				if (dwByte == 0)
					break;
				vecCubePos.push_back(vCubePos);
			}
			CloseHandle(hFile);

			for (auto& iter : vecCubePos)
			{
				_tchar strCubeIndex[64] = { 0 };
				_stprintf_s(strCubeIndex, _T("CubeIndex%d"), iCubeIndex);
				pGameObject = CCube::Create(m_pGraphicDev);
				pGameObject->m_pTransform->m_vInfo[INFO_POS] = iter;
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				FAILED_CHECK_RETURN(pStageLayer->Add_GameObject(strCubeIndex, pGameObject), E_FAIL);
				pGameObject->m_pTransform->m_bIsStatic = true;
				++iCubeIndex;
			}
		}

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
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
