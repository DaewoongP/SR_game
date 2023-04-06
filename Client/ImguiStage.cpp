#include "stdafx.h"
#include "ImguiStage.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "Export_Function.h"

#include "GroundGrid.h"
#include "DefaultCube.h"
#include "InstallCube.h"

CImguiStage::CImguiStage(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev),
	m_bGridON(false), m_bGridCreate(true), m_bCubePlaced(false),
	m_iCubeTextureNumber(0), m_iCubeIndex(0)
{
	m_pDefaultCube = nullptr;

	m_vecGroundGrid.reserve(CUBEX * CUBEY);
}

CImguiStage::~CImguiStage()
{
}

_int CImguiStage::Update_Imgui_Stage(LPDIRECT3DDEVICE9 m_pGraphicDev)
{
	GridMeun(m_pGraphicDev);
	CubeMeun(m_pGraphicDev);

	return S_OK;
}

HRESULT CImguiStage::GridMeun(LPDIRECT3DDEVICE9 m_pGraphicDev)
{
	// 그리드 체크 박스
	ImGui::Checkbox("Grid", &m_bGridON);

	// 그리드 체크 박스 활성화 시 그리드 생성
	if (m_bGridON && m_bGridCreate)
	{
		FAILED_CHECK_RETURN(GroundGridON(m_pGraphicDev, m_vecGroundGrid), E_FAIL);
		m_bGridCreate = false;
	}

	// 그리드 ON
	if (m_bGridON && !m_bGridCreate)
	{
		for (auto& iter : m_vecGroundGrid)
			dynamic_cast<CGroundGrid*>(iter)->Set_GridOn(true);
	}

	// 그리드 OFF
	if (!m_bGridON && !m_bGridCreate)
	{
		for (auto& iter : m_vecGroundGrid)
			dynamic_cast<CGroundGrid*>(iter)->Set_GridOn(false);
	}

	return S_OK;
}

HRESULT CImguiStage::GroundGridON(LPDIRECT3DDEVICE9 m_pGraphicDev, vector<CGameObject*>& vecGrid)
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

HRESULT CImguiStage::CubeMeun(LPDIRECT3DDEVICE9 m_pGraphicDev)
{
	if (ImGui::TreeNode("Cube"))
	{
		// 큐브 설치 체크 박스
		ImGui::Checkbox("Cube Placed", &m_bCubePlaced);

		// 디폴트 큐브 설치
		if (m_bCubePlaced && nullptr == m_pDefaultCube)
			m_pDefaultCube = CreateDefaultCube(m_pGraphicDev);

		// 큐브 모양 선택 콤보 박스
		const char* items[] = { "NONE", "STYLE" };
		ImGui::Combo("Cube Style", &m_iCubeTextureNumber, items, IM_ARRAYSIZE(items));

		// 디폴트 큐브 스타일 선택
		if (m_pDefaultCube)
			dynamic_cast<CDefaultCube*>(m_pDefaultCube)->Set_CubeIndex(m_iCubeTextureNumber);

		// 체크 박스 활성회 시 큐브 설치 부분
		if (m_bCubePlaced && nullptr != m_pDefaultCube)
			CubeInstall(m_pDefaultCube, m_pGraphicDev, m_vecCubeInfo, m_iCubeIndex, m_iCubeTextureNumber);

		// 디폴트 큐브가 생성되어 있는데 체크 항목이 꺼질 경우 디폴트 큐브 사망처리
		if (!m_bCubePlaced && nullptr != m_pDefaultCube)
		{
			m_pDefaultCube->m_bDead = true;
			m_pDefaultCube = nullptr;
		}

		// 저장 기능
		if (ImGui::Button("Cube Save"))
			FAILED_CHECK_RETURN(SaveData(m_vecCubeInfo), E_FAIL);

		// 로드 기능
		ImGui::SameLine();
		if (ImGui::Button("Cube Load"))
			FAILED_CHECK_RETURN(LoadData(m_pGraphicDev, m_vecCubeInfo, m_iCubeIndex), E_FAIL);

		ImGui::TreePop();
	}

	return S_OK;
}

CGameObject * CImguiStage::CreateDefaultCube(LPDIRECT3DDEVICE9 m_pGraphicDev)
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	NULL_CHECK_RETURN(pStageLayer, nullptr);

	CGameObject* pGameObject = nullptr;

	pGameObject = CDefaultCube::Create(m_pGraphicDev);

	NULL_CHECK_RETURN(pGameObject, nullptr);
	FAILED_CHECK_RETURN(pStageLayer->Add_GameObject(L"Cube_Default", pGameObject), nullptr);
	pGameObject->m_pTransform->m_bIsStatic = true;

	return pGameObject;
}

void CImguiStage::CubeInstall(CGameObject * pDefaultCube, LPDIRECT3DDEVICE9 m_pGraphicDev, vector<CUBEINFO>& vecCubeInfo, int & iCubeIndex, int iCubeTextureNumber)
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	NULL_CHECK_RETURN(pStageLayer, );

	CGameObject* pGameObject = nullptr;

	if (Engine::Get_DIMouseState(DIM_LB))
	{
		CUBEINFO tCube = {};

		_tchar strCubeIndex[64] = { 0 };
		_stprintf_s(strCubeIndex, _T("Cube%d"), iCubeIndex);
		pGameObject = CInstallCube::Create(m_pGraphicDev);

		tCube.vCubePos = pGameObject->m_pTransform->m_vInfo[INFO_POS] = pDefaultCube->m_pTransform->m_vInfo[INFO_POS];
		tCube.iCubeTextureNumber = iCubeTextureNumber;
		dynamic_cast<CInstallCube*>(pGameObject)->Set_CubeIndex(tCube.iCubeTextureNumber);

		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pStageLayer->Add_GameObject(strCubeIndex, pGameObject), );
		pGameObject->m_pTransform->m_bIsStatic = true;
		vecCubeInfo.push_back(tCube); // 저장을 위함
		++iCubeIndex;
	}
}

HRESULT CImguiStage::SaveData(vector<CUBEINFO>& vecCubeInfo)
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

HRESULT CImguiStage::LoadData(LPDIRECT3DDEVICE9 m_pGraphicDev, vector<CUBEINFO>& vecCubeInfo, int & iCubeIndex)
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
		pGameObject = CInstallCube::Create(m_pGraphicDev);

		pGameObject->m_pTransform->m_vInfo[INFO_POS] = iter.vCubePos;
		dynamic_cast<CInstallCube*>(pGameObject)->Set_CubeIndex(iter.iCubeTextureNumber);

		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pStageLayer->Add_GameObject(strCubeIndex, pGameObject), E_FAIL);

		pGameObject->m_pTransform->m_bIsStatic = true;
		++iCubeIndex;
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