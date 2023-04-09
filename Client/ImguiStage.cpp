#include "stdafx.h"
#include "ImguiStage.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "Export_Function.h"

#include "GroundGrid.h"
#include "DefaultCube.h"
#include "InstallCube.h"
#include "DefaultGrid.h"
#include "InstallGrid.h"

CImguiStage::CImguiStage(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev),
	m_bGridON(false), m_bGridCreate(true), m_bCubePlaced(false), m_bDefaultGridCreate(false),
	m_iCubeTextureNumber(0), m_iCubeIndex(0), m_iInstallGridIndex(0)
{
	m_pDefaultCube = nullptr;
	m_pDefaultGrid = nullptr;

	m_vecGroundGrid.reserve(CUBEX * CUBEY);
}

CImguiStage::~CImguiStage()
{
}

_int CImguiStage::Update_Imgui_Stage()
{
	GridMeun();
	CubeMeun();

	return S_OK;
}

void CImguiStage::Release()
{
	m_vecGroundGrid.clear();
	m_vecGroundGrid.shrink_to_fit();

	m_vecCubeInfo.clear();
	m_vecCubeInfo.shrink_to_fit();
}

HRESULT CImguiStage::GridMeun()
{
	if (ImGui::TreeNode("Grid"))
	{
		ImGui::Text("Install : F1");

		// 그리드 체크 박스
		ImGui::Checkbox("GroundGrid", &m_bGridON);

		// 그리드 체크 박스 활성화 시 바닥 그리드 생성
		if (m_bGridON && m_bGridCreate)
		{
			FAILED_CHECK_RETURN(GroundGridON(), E_FAIL);
			m_bGridCreate = false;
		}

		// 바닥 그리드 ON
		if (m_bGridON && !m_bGridCreate)
		{
			for (auto& iter : m_vecGroundGrid)
				dynamic_cast<CGroundGrid*>(iter)->Set_GridOn(true);
		}

		// 바닥 그리드 OFF
		if (!m_bGridON && !m_bGridCreate)
		{
			for (auto& iter : m_vecGroundGrid)
				dynamic_cast<CGroundGrid*>(iter)->Set_GridOn(false);
		}

		// 일반 그리드 설치 체크 박스
		ImGui::Checkbox("Grid", &m_bDefaultGridCreate);

		// 디폴트 그리드 설치
		if (m_bDefaultGridCreate && nullptr == m_pDefaultGrid)
		{
			m_pDefaultGrid = CreateDefaultGrid();
			m_bCubePlaced = false;
		}			

		// 체크 박스 활성화 시 그리드 설치 부분
		if (m_bDefaultGridCreate && nullptr != m_pDefaultGrid)
			GridInstall();

		// 디폴트 그리드가 생성되어 있는데 체크 항목이 꺼질 경우 디폴트 그리드 사망처리
		if (!m_bDefaultGridCreate && nullptr != m_pDefaultGrid)
		{
			m_pDefaultGrid->m_bDead = true;
			m_pDefaultGrid = nullptr;
		}

		// 저장 기능
		if (ImGui::Button("Grid Save"))
			FAILED_CHECK_RETURN(SaveGrid(), E_FAIL);

		// 로드 기능
		ImGui::SameLine();
		if (ImGui::Button("Grid Load"))
			FAILED_CHECK_RETURN(LoadGrid(), E_FAIL);

		ImGui::TreePop();
	}

	return S_OK;
}

HRESULT CImguiStage::GroundGridON()
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
			pGameObject = CGroundGrid::Create(m_pGraphicDev, _vec3{ (float)j * 2.f,(float)i * 2.f, 9.f });
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			FAILED_CHECK_RETURN(pStageLayer->Add_GameObject(objName, pGameObject), E_FAIL);
			m_vecGroundGrid.push_back(pGameObject);
		}
	}

	return S_OK;
}

CGameObject * CImguiStage::CreateDefaultGrid()
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	NULL_CHECK_RETURN(pStageLayer, nullptr);

	CGameObject* pGameObject = nullptr;

	pGameObject = CDefaultGrid::Create(m_pGraphicDev, _vec3{ 0.f, 0.f, 10.f });

	NULL_CHECK_RETURN(pGameObject, nullptr);
	FAILED_CHECK_RETURN(pStageLayer->Add_GameObject(L"Grid_Default", pGameObject), nullptr);
	pGameObject->m_pTransform->m_bIsStatic = false;

	return pGameObject;
}

void CImguiStage::GridInstall()
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	NULL_CHECK_RETURN(pStageLayer, );

	CGameObject* pGameObject = nullptr;

	if (Engine::Get_DIKeyState(DIK_F1) == Engine::KEYDOWN)
	{
		OBJINFO tGrid = {};

		_tchar strGridIndex[64] = { 0 };
		_stprintf_s(strGridIndex, _T("Install_Grid%d"), m_iInstallGridIndex);
		pGameObject = CInstallGrid::Create(m_pGraphicDev, m_pDefaultGrid->m_pTransform->m_vInfo[INFO_POS]);

		tGrid.vObjPos = pGameObject->m_pTransform->m_vInfo[INFO_POS];
		tGrid.iObjTypeNumber = m_iInstallGridIndex;

		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pStageLayer->Add_GameObject(strGridIndex, pGameObject), );
		pGameObject->m_pTransform->m_bIsStatic = true;
		m_vecInstallGrid.push_back(tGrid); // 저장을 위함
		++m_iInstallGridIndex;
	}
}

HRESULT CImguiStage::SaveGrid()
{
	HANDLE hFile = CreateFile(L"../Data/GridPos.dat", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD	dwByte = 0;

	for (auto& iter : m_vecInstallGrid)
		WriteFile(hFile, &iter, sizeof(OBJINFO), &dwByte, nullptr);

	CloseHandle(hFile);
	return S_OK;
}

HRESULT CImguiStage::LoadGrid()
{
	m_vecInstallGrid.clear();

	HANDLE hFile = CreateFile(L"../Data/GridPos.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD	dwByte = 0;
	OBJINFO vGrodInfo = {};

	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	NULL_CHECK_RETURN(pStageLayer, E_FAIL);

	CGameObject* pGameObject = nullptr;

	while (true)
	{
		ReadFile(hFile, &vGrodInfo, sizeof(OBJINFO), &dwByte, nullptr);
		if (dwByte == 0)
			break;
		m_vecInstallGrid.push_back(vGrodInfo);
	}
	CloseHandle(hFile);

	for (auto& iter : m_vecInstallGrid)
	{
		_tchar strGridIndex[64] = { 0 };
		_stprintf_s(strGridIndex, _T("GridIndex%d"), m_iInstallGridIndex);
		pGameObject = CInstallGrid::Create(m_pGraphicDev, iter.vObjPos);

		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pStageLayer->Add_GameObject(strGridIndex, pGameObject), E_FAIL);

		pGameObject->m_pTransform->m_bIsStatic = true;
		++m_iInstallGridIndex;
	}

	return S_OK;
}

HRESULT CImguiStage::CubeMeun()
{
	if (ImGui::TreeNode("Cube"))
	{
		ImGui::Text("Install : F1");

		// 큐브 설치 체크 박스
		ImGui::Checkbox("Cube Placed", &m_bCubePlaced);

		// 디폴트 큐브 설치
		if (m_bCubePlaced && nullptr == m_pDefaultCube)
		{
			m_pDefaultCube = CreateDefaultCube();
			m_bDefaultGridCreate = false;
		}			

		// 큐브 모양 선택 콤보 박스
		const char* items[] = { "NONE", "STYLE" };
		ImGui::Combo("Cube Style", &m_iCubeTextureNumber, items, IM_ARRAYSIZE(items));

		// 디폴트 큐브 스타일 선택
		if (m_pDefaultCube)
			dynamic_cast<CDefaultCube*>(m_pDefaultCube)->Set_CubeIndex(m_iCubeTextureNumber);

		// 체크 박스 활성화 시 큐브 설치 부분
		if (m_bCubePlaced && nullptr != m_pDefaultCube)
			CubeInstall();

		// 디폴트 큐브가 생성되어 있는데 체크 항목이 꺼질 경우 디폴트 큐브 사망처리
		if (!m_bCubePlaced && nullptr != m_pDefaultCube)
		{
			m_pDefaultCube->m_bDead = true;
			m_pDefaultCube = nullptr;
		}

		// 저장 기능
		if (ImGui::Button("Cube Save"))
			FAILED_CHECK_RETURN(SaveCube(), E_FAIL);

		// 로드 기능
		ImGui::SameLine();
		if (ImGui::Button("Cube Load"))
			FAILED_CHECK_RETURN(LoadCube(), E_FAIL);

		ImGui::TreePop();
	}

	return S_OK;
}

CGameObject * CImguiStage::CreateDefaultCube()
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	NULL_CHECK_RETURN(pStageLayer, nullptr);

	CGameObject* pGameObject = nullptr;

	pGameObject = CDefaultCube::Create(m_pGraphicDev, _vec3{ 0.f, 0.f, 10.f });

	NULL_CHECK_RETURN(pGameObject, nullptr);
	FAILED_CHECK_RETURN(pStageLayer->Add_GameObject(L"Cube_Default", pGameObject), nullptr);
	pGameObject->m_pTransform->m_bIsStatic = true;

	return pGameObject;
}

void CImguiStage::CubeInstall()
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	NULL_CHECK_RETURN(pStageLayer, );

	CGameObject* pGameObject = nullptr;

	if (Engine::Get_DIKeyState(DIK_F1) == Engine::KEYDOWN)
	{
		OBJINFO tCube = {};

		_tchar strCubeIndex[64] = { 0 };
		_stprintf_s(strCubeIndex, _T("Cube%d"), m_iCubeIndex);
		pGameObject = CInstallCube::Create(m_pGraphicDev, m_pDefaultCube->m_pTransform->m_vInfo[INFO_POS]);

		tCube.vObjPos = pGameObject->m_pTransform->m_vInfo[INFO_POS];
		tCube.iObjTypeNumber = m_iCubeTextureNumber;
		dynamic_cast<CInstallCube*>(pGameObject)->Set_CubeIndex(tCube.iObjTypeNumber);

		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pStageLayer->Add_GameObject(strCubeIndex, pGameObject), );
		pGameObject->m_pTransform->m_bIsStatic = true;
		m_vecCubeInfo.push_back(tCube); // 저장을 위함
		++m_iCubeIndex;
	}
}

HRESULT CImguiStage::SaveCube()
{
	HANDLE hFile = CreateFile(L"../Data/CubePos.dat", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD	dwByte = 0;

	for (auto& iter : m_vecCubeInfo)
		WriteFile(hFile, &iter, sizeof(OBJINFO), &dwByte, nullptr);

	CloseHandle(hFile);
	return S_OK;
}

HRESULT CImguiStage::LoadCube()
{
	m_vecCubeInfo.clear();

	HANDLE hFile = CreateFile(L"../Data/CubePos.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD	dwByte = 0;
	OBJINFO vCubeInfo = {};

	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	NULL_CHECK_RETURN(pStageLayer, E_FAIL);

	CGameObject* pGameObject = nullptr;

	while (true)
	{
		ReadFile(hFile, &vCubeInfo, sizeof(OBJINFO), &dwByte, nullptr);
		if (dwByte == 0)
			break;
		m_vecCubeInfo.push_back(vCubeInfo);
	}
	CloseHandle(hFile);

	for (auto& iter : m_vecCubeInfo)
	{
		_tchar strCubeIndex[64] = { 0 };
		_stprintf_s(strCubeIndex, _T("CubeIndex%d"), m_iCubeIndex);
		pGameObject = CInstallCube::Create(m_pGraphicDev, iter.vObjPos);

		dynamic_cast<CInstallCube*>(pGameObject)->Set_CubeIndex(iter.iObjTypeNumber);

		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pStageLayer->Add_GameObject(strCubeIndex, pGameObject), E_FAIL);

		pGameObject->m_pTransform->m_bIsStatic = true;
		++m_iCubeIndex;
	}

	return S_OK;
}

CImguiStage * CImguiStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CImguiStage* pInstance = new CImguiStage(pGraphicDev);

	if (nullptr == pInstance)
		return nullptr;

	return pInstance;
}