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
	// �׸��� üũ �ڽ�
	ImGui::Checkbox("Grid", &m_bGridON);

	// �׸��� üũ �ڽ� Ȱ��ȭ �� �׸��� ����
	if (m_bGridON && m_bGridCreate)
	{
		FAILED_CHECK_RETURN(GroundGridON(m_vecGroundGrid), E_FAIL);
		m_bGridCreate = false;
	}

	// �׸��� ON
	if (m_bGridON && !m_bGridCreate)
	{
		for (auto& iter : m_vecGroundGrid)
			dynamic_cast<CGroundGrid*>(iter)->Set_GridOn(true);
	}

	// �׸��� OFF
	if (!m_bGridON && !m_bGridCreate)
	{
		for (auto& iter : m_vecGroundGrid)
			dynamic_cast<CGroundGrid*>(iter)->Set_GridOn(false);
	}

	return S_OK;
}

HRESULT CImguiStage::GroundGridON(vector<CGameObject*>& vecGrid)
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

HRESULT CImguiStage::CubeMeun()
{
	if (ImGui::TreeNode("Cube"))
	{
		// ť�� ��ġ üũ �ڽ�
		ImGui::Checkbox("Cube Placed", &m_bCubePlaced);

		// ����Ʈ ť�� ��ġ
		if (m_bCubePlaced && nullptr == m_pDefaultCube)
			m_pDefaultCube = CreateDefaultCube();

		// ť�� ��� ���� �޺� �ڽ�
		const char* items[] = { "NONE", "STYLE" };
		ImGui::Combo("Cube Style", &m_iCubeTextureNumber, items, IM_ARRAYSIZE(items));

		// ����Ʈ ť�� ��Ÿ�� ����
		if (m_pDefaultCube)
			dynamic_cast<CDefaultCube*>(m_pDefaultCube)->Set_CubeIndex(m_iCubeTextureNumber);

		// üũ �ڽ� Ȱ��ȭ �� ť�� ��ġ �κ�
		if (m_bCubePlaced && nullptr != m_pDefaultCube)
			CubeInstall(m_pDefaultCube, m_vecCubeInfo, m_iCubeIndex, m_iCubeTextureNumber);

		// ����Ʈ ť�갡 �����Ǿ� �ִµ� üũ �׸��� ���� ��� ����Ʈ ť�� ���ó��
		if (!m_bCubePlaced && nullptr != m_pDefaultCube)
		{
			m_pDefaultCube->m_bDead = true;
			m_pDefaultCube = nullptr;
		}

		// ���� ���
		if (ImGui::Button("Cube Save"))
			FAILED_CHECK_RETURN(SaveCube(m_vecCubeInfo), E_FAIL);

		// �ε� ���
		ImGui::SameLine();
		if (ImGui::Button("Cube Load"))
			FAILED_CHECK_RETURN(LoadCube(m_vecCubeInfo, m_iCubeIndex), E_FAIL);

		ImGui::TreePop();
	}

	return S_OK;
}

CGameObject * CImguiStage::CreateDefaultCube()
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

void CImguiStage::CubeInstall(CGameObject * pDefaultCube, vector<CUBEINFO>& vecCubeInfo, int & iCubeIndex, int iCubeTextureNumber)
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	NULL_CHECK_RETURN(pStageLayer, );

	CGameObject* pGameObject = nullptr;

	if (Engine::Get_DIKeyState(DIK_F1) == Engine::KEYDOWN)
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
		vecCubeInfo.push_back(tCube); // ������ ����
		++iCubeIndex;
	}
}

HRESULT CImguiStage::SaveCube(vector<CUBEINFO>& vecCubeInfo)
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

HRESULT CImguiStage::LoadCube(vector<CUBEINFO>& vecCubeInfo, int & iCubeIndex)
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