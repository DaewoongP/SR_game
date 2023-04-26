#include "stdafx.h"
#include "ImguiStage.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

#include "GroundGrid.h"
#include "DefaultCube.h"
#include "InstallCube.h"
#include "DefaultGrid.h"
#include "InstallGrid.h"

bool CImguiStage::m_bGridON = false;
static _vec3 vPos;
static _vec3 m_vPos;
CImguiStage::CImguiStage(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev),
	m_bGridCreate(true), m_bCubePlaced(false), m_bDefaultGridCreate(false),
	m_iCubeTextureNumber(0)
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
	GridMenu();
	CubeMenu();

	if (ImGui::Button("Undo"))
		if (Undo(m_iStageNumber) != S_OK)
			MSG_BOX("Undo Failed");
	return S_OK;
}

void CImguiStage::Release()
{
	m_vecGroundGrid.clear();
	m_vecGroundGrid.shrink_to_fit();

	m_vecCubeInfo.clear();
	m_vecCubeInfo.shrink_to_fit();
	m_vecGameObject.clear();
}

HRESULT CImguiStage::GridMenu()
{
	if (ImGui::TreeNode("Grid"))
	{
		ImGui::Text("Install : F1");

		// �׸��� üũ �ڽ�
		ImGui::Checkbox("GroundGrid", &m_bGridON);

		// �׸��� üũ �ڽ� Ȱ��ȭ �� �ٴ� �׸��� ����
		if (m_bGridON && m_bGridCreate)
		{
			GroundGridON();
			m_bGridCreate = false;
		}

		// �Ϲ� �׸��� ��ġ üũ �ڽ�
		ImGui::Checkbox("Grid", &m_bDefaultGridCreate);

		// ����Ʈ �׸��� ��ġ
		if (m_bDefaultGridCreate && nullptr == m_pDefaultGrid)
		{
			CreateDefaultGrid();
			m_bCubePlaced = false;
		}			

		// üũ �ڽ� Ȱ��ȭ �� �׸��� ��ġ �κ�
		if (m_bDefaultGridCreate && nullptr != m_pDefaultGrid)
			GridInstall();

		// ����Ʈ �׸��尡 �����Ǿ� �ִµ� üũ �׸��� ���� ��� ����Ʈ �׸��� ���ó��
		if (!m_bDefaultGridCreate && nullptr != m_pDefaultGrid)
		{
			m_pDefaultGrid->m_bDead = true;
			m_pDefaultGrid = nullptr;
		}
		Scale();
		// ���� ���
		if (ImGui::Button("Grid Save"))
			FAILED_CHECK_RETURN(SaveGrid(m_iStageNumber), E_FAIL);

		// �ε� ���
		ImGui::SameLine();
		if (ImGui::Button("Grid Load"))
			FAILED_CHECK_RETURN(LoadGrid(m_iStageNumber), E_FAIL);

		ImGui::TreePop();
	}


	return S_OK;
}

void CImguiStage::GroundGridON()
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	NULL_CHECK_RETURN(pStageLayer, );

	CGameObject* pGameObject = nullptr;

	for (int i = 0; i < CUBEY; ++i)
	{
		for (int j = 0; j < CUBEX; ++j)
		{
			FAILED_CHECK_RETURN(FACTORY<CGroundGrid>::Create(L"GroundGrid", pStageLayer,
				_vec3{ (float)j * 2.f,(float)i * 2.f, 10.f }), );
		}
	}

	return;
}

void CImguiStage::CreateDefaultGrid()
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	NULL_CHECK_RETURN(pStageLayer, );

	FAILED_CHECK_RETURN(FACTORY<CDefaultGrid>::Create(L"DefaultGrid", pStageLayer,
		_vec3{ 0.f, 0.f, 0.f}), );

	m_pDefaultGrid = Engine::Get_GameObject(L"Layer_GameLogic", L"DefaultGrid");

	return;
}

void CImguiStage::GridInstall()
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	NULL_CHECK_RETURN(pStageLayer, );

	if (Engine::Get_DIKeyState(DIK_F1) == Engine::KEYDOWN)
	{
		if (!m_vecInstallGrid.empty())
		{
			m_vecInstallGrid.back().vObjPos += vPos;
		}
		vPos = { 0.0f,0.0f,-0.0f };
		OBJINFO tGrid = {};
		MakeGameObject(pStageLayer, L"InstallGrid");

		tGrid.vObjPos = m_vecGameObject.back()->m_pTransform->m_vInfo[INFO_POS];//m_pDefaultGrid->m_pTransform->m_vInfo[INFO_POS];
		tGrid.iObjTypeNumber = 0;

		m_vecInstallGrid.push_back(tGrid); // ������ ����

	}
	if (!m_vecGameObject.empty())
	{
		CGameObject* DynamicPos = m_vecGameObject.back();
		m_vPos = m_vecInstallGrid.back().vObjPos;
		if (DynamicPos != nullptr)
		{
			m_vecGameObject.back()->m_pTransform->m_vInfo[INFO_POS] = m_vPos + vPos;
		}
	}
}
void CImguiStage::Scale()
{
	
	ImGui::PushItemWidth(100);

	ImGui::DragFloat("X", &vPos.x);
	ImGui::SameLine();
	ImGui::DragFloat("Y", &vPos.y);
	ImGui::SameLine();
	ImGui::DragFloat("Z", &vPos.z, 0.05f, -10.0f, 10.0f);


}
HRESULT CImguiStage::SaveGrid(_int iStageNumber)
{
	TCHAR dataFile[128] = { 0 };
	_stprintf_s(dataFile, _T("../Data/GridPos%d.dat"), (iStageNumber + 1));

	HANDLE hFile = CreateFile(dataFile, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD	dwByte = 0;
	if (!m_vecInstallGrid.empty())
		m_vecInstallGrid.back().vObjPos += vPos;


	for (auto& iter : m_vecInstallGrid)
		WriteFile(hFile, &iter, sizeof(OBJINFO), &dwByte, nullptr);

	CloseHandle(hFile);
	return S_OK;
}

HRESULT CImguiStage::LoadGrid(_int iStageNumber, CScene* pScene)
{
	m_vecInstallGrid.clear();
	m_vecGameObject.clear();
	TCHAR dataFile[128] = { 0 };
	_stprintf_s(dataFile, _T("../Data/GridPos%d.dat"), (iStageNumber + 1));

	HANDLE hFile = CreateFile(dataFile, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD	dwByte = 0;
	OBJINFO vGrodInfo = {};

	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	if (pStageLayer == nullptr)
		pStageLayer = pScene->Get_Layer(L"Layer_GameLogic");

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
		FAILED_CHECK_RETURN(FACTORY<CInstallGrid>::Create(L"InstallGrid", pStageLayer,
			iter.vObjPos), E_FAIL);
	}

	return S_OK;
}

HRESULT CImguiStage::CubeMenu()
{
	if (ImGui::TreeNode("Cube"))
	{
		ImGui::Text("Install : F1");

		// ť�� ��ġ üũ �ڽ�
		ImGui::Checkbox("Cube Placed", &m_bCubePlaced);

		// ����Ʈ ť�� ��ġ
		if (m_bCubePlaced && nullptr == m_pDefaultCube)
		{
			CreateDefaultCube();
			m_bDefaultGridCreate = false;
		}

		// ť�� ��� ���� �޺� �ڽ�
		const char* items[] = { "STAGE1_0", "STAGE1_1", "STAGE2_0", "STAGE2_1", "STAGE3_0", "STAGE3_1", "STAGE4_0", "STAGE4_1", "INVISIBILITY CUBE"};
		ImGui::Combo("Cube Style", &m_iCubeTextureNumber, items, IM_ARRAYSIZE(items));

		// ����Ʈ ť�� ��Ÿ�� ����
		if (m_pDefaultCube)
			dynamic_cast<CDefaultCube*>(m_pDefaultCube)->Set_CubeIndex(m_iCubeTextureNumber);

		// üũ �ڽ� Ȱ��ȭ �� ť�� ��ġ �κ�
		if (m_bCubePlaced && nullptr != m_pDefaultCube)
			CubeInstall();

		// ����Ʈ ť�갡 �����Ǿ� �ִµ� üũ �׸��� ���� ��� ����Ʈ ť�� ���ó��
		if (!m_bCubePlaced && nullptr != m_pDefaultCube)
		{
			m_pDefaultCube->m_bDead = true;
			m_pDefaultCube = nullptr;
		}
		Scale();
		// ���� ���
		if (ImGui::Button("Cube Save"))
			FAILED_CHECK_RETURN(SaveCube(m_iStageNumber), E_FAIL);

		// �ε� ���
		ImGui::SameLine();
		if (ImGui::Button("Cube Load"))
			FAILED_CHECK_RETURN(LoadCube(m_iStageNumber), E_FAIL);


		ImGui::TreePop();
	}

	return S_OK;
}

void CImguiStage::CreateDefaultCube()
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	NULL_CHECK_RETURN(pStageLayer, );

	FAILED_CHECK_RETURN(FACTORY<CDefaultCube>::Create(L"DefaultCube", pStageLayer,
		_vec3{ 0.f, 0.f, 0.f }), );

	m_pDefaultCube = Engine::Get_GameObject(L"Layer_GameLogic", L"DefaultCube");

	return;
}

void CImguiStage::CubeInstall()
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	NULL_CHECK_RETURN(pStageLayer, );

	if (Engine::Get_DIKeyState(DIK_F1) == Engine::KEYDOWN)
	{
		if (!m_vecCubeInfo.empty())
			m_vecCubeInfo.back().vObjPos += vPos;
		OBJINFO tCube = {};

		MakeGameObject(pStageLayer, L"InstallCube", m_iCubeTextureNumber);

		tCube.vObjPos = m_pDefaultCube->m_pTransform->m_vInfo[INFO_POS];
		tCube.iObjTypeNumber = m_iCubeTextureNumber;

		m_vecCubeInfo.push_back(tCube); // ������ ����
	}
	if (!m_vecGameObject.empty())
	{
		CGameObject* DynamicPos = m_vecGameObject.back();
		m_vPos = m_vecCubeInfo.back().vObjPos;
		if (DynamicPos != nullptr)
		{
			m_vecGameObject.back()->m_pTransform->m_vInfo[INFO_POS] = m_vPos + vPos;
		}
	}
}

HRESULT CImguiStage::SaveCube(_int iStageNumber)
{
	TCHAR dataFile[128] = { 0 };
	_stprintf_s(dataFile, _T("../Data/CubePos%d.dat"), (iStageNumber + 1));

	HANDLE hFile = CreateFile(dataFile, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD	dwByte = 0;
	if (!m_vecCubeInfo.empty())
		m_vecCubeInfo.back().vObjPos += vPos;
	
	for (auto& iter : m_vecCubeInfo)
		WriteFile(hFile, &iter, sizeof(OBJINFO), &dwByte, nullptr);

	CloseHandle(hFile);
	return S_OK;
}

HRESULT CImguiStage::LoadCube(_int iStageNumber, CScene* pScene)
{
	m_vecCubeInfo.clear();
	m_vecGameObject.clear();
	TCHAR dataFile[128] = { 0 };
	_stprintf_s(dataFile, _T("../Data/CubePos%d.dat"), (iStageNumber + 1));

	HANDLE hFile = CreateFile(dataFile, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD	dwByte = 0;
	OBJINFO vCubeInfo = {};

	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	if (pStageLayer == nullptr)
		pStageLayer = pScene->Get_Layer(L"Layer_GameLogic");

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
		FAILED_CHECK_RETURN(FACTORY<CInstallCube>::Create(L"InstallCube", pStageLayer,
			iter.vObjPos, iter.iObjTypeNumber), E_FAIL);
	}

	return S_OK;
}

HRESULT CImguiStage::Undo(_int iStageNumber)
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	NULL_CHECK_RETURN(pStageLayer, E_FAIL);
	if (m_vecGameObject.empty())
		return E_FAIL;
	CGameObject* pGameObject = m_vecGameObject.back();
	if (!lstrcmp(pGameObject->m_pTag, L"InstallCube"))
	{
		if (m_vecCubeInfo.empty())
			return E_FAIL;
		m_vecCubeInfo.pop_back();
	}
	else if (!lstrcmp(pGameObject->m_pTag, L"InstallGrid"))
	{
		if (m_vecInstallGrid.empty())
			return E_FAIL;
		m_vecInstallGrid.pop_back();
	}
	pStageLayer->Delete_LastObject(pGameObject);
	m_vecGameObject.pop_back();
	return S_OK;
}

CImguiStage * CImguiStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CImguiStage* pInstance = new CImguiStage(pGraphicDev);

	if (nullptr == pInstance)
		return nullptr;

	return pInstance;
}

void CImguiStage::MakeGameObject(CLayer * pLayer, const _tchar * pObjTag)
{
	CGameObject* pGameObject = nullptr;
	pGameObject = CInstallGrid::Create(m_pGraphicDev,
		m_pDefaultGrid->m_pTransform->m_vInfo[INFO_POS]);
	if (pGameObject == nullptr)
		return;
	pGameObject->Sort_Component();
	pLayer->Add_GameObject(pObjTag, pGameObject);
	m_vecGameObject.push_back(pGameObject);
}

void CImguiStage::MakeGameObject(CLayer * pLayer, const _tchar * pObjTag, _int iNum)
{
	 CGameObject* pGameObject = CInstallCube::Create(m_pGraphicDev,
		m_pDefaultCube->m_pTransform->m_vInfo[INFO_POS], iNum);
	if (pGameObject == nullptr)
		return;
	pGameObject->Sort_Component();
	pLayer->Add_GameObject(pObjTag, pGameObject);
	m_vecGameObject.push_back(pGameObject);
}
