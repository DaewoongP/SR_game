#include "stdafx.h"
#include "ImguiMgr.h"
#include"imgui.h"
#include"imgui_impl_dx9.h"
#include"imgui_impl_win32.h"
#include"..\Engine\Export_Function.h"
#include "Cube.h"
#include "GroundGrid.h"

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
		// 큐브 설치 관련 변수
		static vector<CUBEINFO> vecCubeInfo; // 저장을 위한 큐브 벡터
		static CGameObject* pDefaultCube = nullptr; // 디폴트 큐브 형성
		static bool bCubePlaced = false;
		static int iCubeIndex = 0;

		// 그리드 관련 변수
		static vector<CGameObject*> vecGroundGrid;
		vecGroundGrid.reserve(CUBEX * CUBEY);
		static bool bGridON = false;
		static bool bGridCreate = true;

		// 본문 시작
		ImGui::Begin("Hello,Imgui!");

		ImGui::Checkbox("Demo Window", &show_demo_window);

		// 그리드 관련
		{
			// 그리드 체크 박스
			ImGui::Checkbox("Grid", &bGridON);

			// 그리드 체크 박스 활성화 시 그리드 생성
			if (bGridON && bGridCreate)
			{
				FAILED_CHECK_RETURN(GroundGridON(m_pGraphicDev, vecGroundGrid), E_FAIL);
				bGridCreate = false;
			}

			// 그리드 ON
			if (bGridON && !bGridCreate)
			{
				for (auto& iter : vecGroundGrid)
					dynamic_cast<CGroundGrid*>(iter)->Set_GridOn(true);
			}

			// 그리드 OFF
			if (!bGridON && !bGridCreate)
			{
				for (auto& iter : vecGroundGrid)
					dynamic_cast<CGroundGrid*>(iter)->Set_GridOn(false);
			}
		}

		if (ImGui::TreeNode("Cube"))
		{
			// 큐브 설치 체크 박스
			ImGui::Checkbox("Cube Placed", &bCubePlaced);

			// 디폴트 큐브, 그리드가 없으면 생성
			if (bCubePlaced && nullptr == pDefaultCube)
				pDefaultCube = CreateDefaultCube(m_pGraphicDev);			

			// 체크 박스 활성회 시 큐브 설치 부분
			if (bCubePlaced)
				CubeInstall(pDefaultCube, m_pGraphicDev, vecCubeInfo, iCubeIndex);

			// 디폴트 큐브가 생성되어 있는데 체크 항목이 꺼질 경우 디폴트 큐브 사망처리
			if (!bCubePlaced && nullptr != pDefaultCube)
			{
				pDefaultCube->m_bDead = true;
				pDefaultCube = nullptr;
			}

			// 저장 기능
			if (ImGui::Button("Cube Save"))
				FAILED_CHECK_RETURN(SaveData(vecCubeInfo), E_FAIL);

			// 로드 기능
			ImGui::SameLine();
			if (ImGui::Button("Cube Load"))
				FAILED_CHECK_RETURN(LoadData(m_pGraphicDev, vecCubeInfo, iCubeIndex), E_FAIL);

			ImGui::TreePop();
		}		
		// 마우스 커서 위치
		if (ImGui::IsMousePosValid())
			ImGui::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);

		int x = 1366 / CUBEX * 2;
		int y = 768 / CUBEY * 2;

		int nx = 1366 / x;
		int ny = 768 / y;

		// 마우스 피킹 관련
		{
			if (pDefaultCube)
			{
				float Gridx = io.MousePos.x / nx / (1.3f) - 1.f;
				float Gridy = (768 / ny - io.MousePos.y / ny) / (2.35f) - 1.f;

				int installx = Gridx / 2;
				int instally = Gridy / 2;

				pDefaultCube->m_pTransform->m_vInfo[INFO_POS].x = installx * 2;
				pDefaultCube->m_pTransform->m_vInfo[INFO_POS].y = instally * 2;
				pDefaultCube->m_pTransform->m_vInfo[INFO_POS].z = 10.f;
				
				if (Engine::Get_DIKeyState(DIK_E) == Engine::KEYDOWN)
				{
					CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
					NULL_CHECK_RETURN(pStageLayer, );

					CGameObject* pGameObject = nullptr;

					CUBEINFO tCube = {};

					_tchar strCubeIndex[64] = { 0 };
					_stprintf_s(strCubeIndex, _T("Cube%d"), iCubeIndex);
					pGameObject = CCube::Create(m_pGraphicDev);

					NULL_CHECK_RETURN(pGameObject, );
					FAILED_CHECK_RETURN(pStageLayer->Add_GameObject(strCubeIndex, pGameObject), );
					pGameObject->m_pTransform->m_bIsStatic = true;
					vecCubeInfo.push_back(tCube); // 저장을 위함
					++iCubeIndex;
				}
			}
		}

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
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

HRESULT CImguiMgr::SaveData(vector<CUBEINFO>& vecCubeInfo)
{
	HANDLE hFile = CreateFile(L"../Data/CubePos.dat", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD	dwByte = 0;

	for (auto& iter : vecCubeInfo)
		WriteFile(hFile, &iter, sizeof(CUBEINFO), &dwByte, nullptr);

	CloseHandle(hFile);
	return S_OK;
}

HRESULT CImguiMgr::LoadData(LPDIRECT3DDEVICE9 m_pGraphicDev, vector<CUBEINFO>& vecCubeInfo, int iCubeIndex)
{
	vecCubeInfo.clear();

	HANDLE hFile = CreateFile(L"../Data/CubePos.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD	dwByte = 0;
	CUBEINFO vCubeInfo = {};

	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	NULL_CHECK_RETURN(pStageLayer, E_FAIL);

	CGameObject* pGameObject = nullptr;

	while (true)
	{
		ReadFile(hFile, &vCubeInfo, sizeof(CUBEINFO), &dwByte, nullptr);
		if (dwByte == 0)
			break;
		vecCubeInfo.push_back(vCubeInfo);
	}
	CloseHandle(hFile);

	for (auto& iter : vecCubeInfo)
	{
		_tchar strCubeIndex[64] = { 0 };
		_stprintf_s(strCubeIndex, _T("CubeIndex%d"), iCubeIndex);
		pGameObject = CCube::Create(m_pGraphicDev);

		pGameObject->m_pTransform->m_vInfo[INFO_POS] = iter.vCubePos;
		//dynamic_cast<CCube*>(pGameObject)->Set_CubeTextureIndex(iter.iCubeTextureNumber);

		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pStageLayer->Add_GameObject(strCubeIndex, pGameObject), E_FAIL);

		pGameObject->m_pTransform->m_bIsStatic = true;
		++iCubeIndex;
	}

	return S_OK;
}

CGameObject* CImguiMgr::CreateDefaultCube(LPDIRECT3DDEVICE9 m_pGraphicDev)
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	NULL_CHECK_RETURN(pStageLayer, nullptr);

	CGameObject* pGameObject = nullptr;

	pGameObject = CCube::Create(m_pGraphicDev);
	pGameObject->m_pTransform->m_vInfo[INFO_POS] = _vec3{ 10.f, 10.f, 10.f };

	NULL_CHECK_RETURN(pGameObject, nullptr);
	FAILED_CHECK_RETURN(pStageLayer->Add_GameObject(L"Cube_Default", pGameObject), nullptr);
	pGameObject->m_pTransform->m_bIsStatic = true;

	return pGameObject;
}

void CImguiMgr::CubeInstall(CGameObject* pDefaultCube, LPDIRECT3DDEVICE9 m_pGraphicDev, vector<CUBEINFO>& vecCubeInfo, int iCubeIndex)
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	NULL_CHECK_RETURN(pStageLayer, );

	CGameObject* pGameObject = nullptr;

	// 디폴트 큐브의 움직임(한칸씩)
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
		CUBEINFO tCube = {};

		_tchar strCubeIndex[64] = { 0 };
		_stprintf_s(strCubeIndex, _T("Cube%d"), iCubeIndex);
		pGameObject = CCube::Create(m_pGraphicDev);

		tCube.vCubePos = pGameObject->m_pTransform->m_vInfo[INFO_POS] = pDefaultCube->m_pTransform->m_vInfo[INFO_POS];
		//tCube.iCubeTextureNumber = dynamic_cast<CCube*>(pDefaultCube)->Get_CubeTextureIndex();
		//dynamic_cast<CCube*>(pGameObject)->Set_CubeTextureIndex(tCube.iCubeTextureNumber);

		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pStageLayer->Add_GameObject(strCubeIndex, pGameObject), );
		pGameObject->m_pTransform->m_bIsStatic = true;
		vecCubeInfo.push_back(tCube); // 저장을 위함
		++iCubeIndex;
	}
}

HRESULT CImguiMgr::GroundGridON(LPDIRECT3DDEVICE9 m_pGraphicDev, vector<CGameObject*>& vecGrid)
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
			_stprintf_s(objName, _T("Grid%d"), (iGridIndex));
			pGameObject = CGroundGrid::Create(m_pGraphicDev);
			pGameObject->m_pTransform->m_vInfo[INFO_POS] = _vec3{ (float)j * 2.f,(float)i * 2.f, 10.f };
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			FAILED_CHECK_RETURN(pStageLayer->Add_GameObject(objName, pGameObject), E_FAIL);
			vecGrid.push_back(pGameObject);
		}
	}

	return S_OK;
}