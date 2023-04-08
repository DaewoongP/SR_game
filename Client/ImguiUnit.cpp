#include "stdafx.h"
#include "ImguiUnit.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "Export_Function.h"

#include "DefaultMonster.h"
#include "Pig.h"
#include "Bat.h"
#include "Portal.h"
#include "Key.h"
#include "KeyBox.h"
#include "MoveBox.h"

CImguiUnit::CImguiUnit(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev),
	m_bMonsterON(false), m_bMapObjectON(false),
	m_iMonsterType(0), m_iMonsterindex(0), m_iMapObjectType(0), m_iMapObjectIndex(0)
{
	m_pDefaultMonster = nullptr;
	m_pDefaultMapObject = nullptr;
}

CImguiUnit::~CImguiUnit()
{
}

_int CImguiUnit::Update_Imgui_Unit()
{
	MonsterMenu();
	MapObjectMenu();

	return S_OK;
}

void CImguiUnit::Release()
{
}

HRESULT CImguiUnit::MonsterMenu()
{
	if (ImGui::TreeNode("Monster"))
	{
		// 몬스터 설치 체크 박스
		ImGui::Checkbox("Monster Placed", &m_bMonsterON);

		// 몬스터 종류 선택 콤보 박스
		const char* items[] = { "PIG", "BAT" };
		ImGui::Combo("Monster Type", &m_iMonsterType, items, IM_ARRAYSIZE(items));

		// 체크 박스가 켜졌을 때 디폴트 몬스터 생성
		if (m_bMonsterON && nullptr == m_pDefaultMonster)
		{
			m_pDefaultMonster = CreateDefaultMonster();

			if (m_bMapObjectON)
				m_bMapObjectON = false;
		}			

		// 체크 박스 활성화 시 몬스터 설치 부분
		if (m_bMonsterON && nullptr != m_pDefaultMonster)
			MonsterInstall();

		// 체크 박스가 꺼졌을 때 디폴트 몬스터 사망 처리
		if (!m_bMonsterON && nullptr != m_pDefaultMonster)
		{
			m_pDefaultMonster->m_bDead = true;
			m_pDefaultMonster = nullptr;
		}

		// 저장 기능
		if (ImGui::Button("Monster Save"))
			FAILED_CHECK_RETURN(SaveMonster(), E_FAIL);

		// 로드 기능
		ImGui::SameLine();
		if (ImGui::Button("Monster Load"))
			FAILED_CHECK_RETURN(LoadMonster(), E_FAIL);

		ImGui::TreePop();
	}

	return S_OK;
}

CGameObject * CImguiUnit::CreateDefaultMonster()
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	NULL_CHECK_RETURN(pStageLayer, nullptr);

	CGameObject* pGameObject = nullptr;

	pGameObject = CDefaultMonster::Create(m_pGraphicDev, _vec3{ 0.f, 0.f, 0.f });
	dynamic_cast<CDefaultMonster*>(pGameObject)->Set_DefaultIndex(CDefaultMonster::DMonster);

	NULL_CHECK_RETURN(pGameObject, nullptr);
	FAILED_CHECK_RETURN(pStageLayer->Add_GameObject(L"Monster_Default", pGameObject), nullptr);

	return pGameObject;
}

void CImguiUnit::MonsterInstall()
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	NULL_CHECK_RETURN(pStageLayer, );

	CGameObject* pGameObject = nullptr;

	if (Engine::Get_DIKeyState(DIK_F2) == Engine::KEYDOWN)
	{
		OBJINFO tMonsterInfo = {};
		_tchar strCubeIndex[64] = { 0 };

		if (0 == m_iMonsterType) // 돼지
		{
			_stprintf_s(strCubeIndex, _T("Pig%d"), m_iMonsterindex);
			pGameObject = CPig::Create(m_pGraphicDev, m_pDefaultMonster->m_pTransform->m_vInfo[INFO_POS]);
		}

		else if (1 == m_iMonsterType) // 박쥐
		{
			_stprintf_s(strCubeIndex, _T("Bat%d"), m_iMonsterindex);
			pGameObject = CBat::Create(m_pGraphicDev, m_pDefaultMonster->m_pTransform->m_vInfo[INFO_POS]);
		}

		tMonsterInfo.vObjPos = pGameObject->m_pTransform->m_vInfo[INFO_POS];
		tMonsterInfo.iObjTypeNumber = m_iMonsterType;

		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pStageLayer->Add_GameObject(strCubeIndex, pGameObject), );
		pGameObject->m_pTransform->m_bIsStatic = false;
		m_vecMonsterInfo.push_back(tMonsterInfo);
		++m_iMonsterindex;
	}
}

HRESULT CImguiUnit::SaveMonster()
{
	HANDLE hFile = CreateFile(L"../Data/MonsterPos.dat", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD	dwByte = 0;

	for (auto& iter : m_vecMonsterInfo)
		WriteFile(hFile, &iter, sizeof(OBJINFO), &dwByte, nullptr);

	CloseHandle(hFile);
	return S_OK;
}

HRESULT CImguiUnit::LoadMonster()
{
	m_vecMonsterInfo.clear();

	HANDLE hFile = CreateFile(L"../Data/MonsterPos.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD	dwByte = 0;
	OBJINFO vMonsterInfo = {};

	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	NULL_CHECK_RETURN(pStageLayer, E_FAIL);

	CGameObject* pGameObject = nullptr;

	while (true)
	{
		ReadFile(hFile, &vMonsterInfo, sizeof(OBJINFO), &dwByte, nullptr);
		if (dwByte == 0)
			break;
		m_vecMonsterInfo.push_back(vMonsterInfo);
	}
	CloseHandle(hFile);

	for (auto& iter : m_vecMonsterInfo)
	{
		_tchar strCubeIndex[64] = { 0 };

		// 돼지
		if (0 == iter.iObjTypeNumber)
		{
			_stprintf_s(strCubeIndex, _T("Pig%d"), m_iMonsterindex);
			pGameObject = CPig::Create(m_pGraphicDev, iter.vObjPos);			
		}

		// 박쥐
		else if (1 == iter.iObjTypeNumber)
		{
			_stprintf_s(strCubeIndex, _T("Pig%d"), m_iMonsterindex);
			pGameObject = CBat::Create(m_pGraphicDev, iter.vObjPos);
		}

		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pStageLayer->Add_GameObject(strCubeIndex, pGameObject), E_FAIL);

		pGameObject->m_pTransform->m_bIsStatic = false;
		++m_iMonsterindex;
	}

	return S_OK;
}

HRESULT CImguiUnit::MapObjectMenu()
{
	if (ImGui::TreeNode("Map Object"))
	{
		// 맵 오브젝트 설치 체크 박스
		ImGui::Checkbox("Map Object Placed", &m_bMapObjectON);

		// 맵 오브젝트 종류 선택 콤보 박스
		const char* items[] = { "KEY", "KEY BOX", "MOVE BOX", "PORTAL" };
		ImGui::Combo("Map Object Type", &m_iMapObjectType, items, IM_ARRAYSIZE(items));

		// 체크 박스가 켜졌을 때 디폴트 맵 오브젝트 생성
		if (m_bMapObjectON && nullptr == m_pDefaultMapObject)
		{
			m_pDefaultMapObject = CreateDefaultMapObject();

			if (m_bMonsterON)
				m_bMonsterON = false;
		}			

		// 체크 박스 활성화 시 맵 오브젝트 설치 부분
		if (m_bMapObjectON && nullptr != m_pDefaultMapObject)
			MapObjectInstall();

		// 체크 박스가 꺼졌을 때 디폴트 맵 오브젝트 사망 처리
		if (!m_bMapObjectON && nullptr != m_pDefaultMapObject)
		{
			m_pDefaultMapObject->m_bDead = true;
			m_pDefaultMapObject = nullptr;
		}

		// 저장 기능
		if (ImGui::Button("MapObject Save"))
			FAILED_CHECK_RETURN(SaveMapObject(), E_FAIL);

		// 로드 기능
		ImGui::SameLine();
		if (ImGui::Button("MapObject Load"))
			FAILED_CHECK_RETURN(LoadMapObject(), E_FAIL);

		ImGui::TreePop();
	}

	return S_OK;
}

CGameObject * CImguiUnit::CreateDefaultMapObject()
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	NULL_CHECK_RETURN(pStageLayer, nullptr);

	CGameObject* pGameObject = nullptr;

	pGameObject = CDefaultMonster::Create(m_pGraphicDev, _vec3{ 0.f, 0.f, 0.f });
	dynamic_cast<CDefaultMonster*>(pGameObject)->Set_DefaultIndex(CDefaultMonster::DMapObject);

	NULL_CHECK_RETURN(pGameObject, nullptr);
	FAILED_CHECK_RETURN(pStageLayer->Add_GameObject(L"MapObject_Default", pGameObject), nullptr);

	return pGameObject;
}

void CImguiUnit::MapObjectInstall()
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	NULL_CHECK_RETURN(pStageLayer, );

	CGameObject* pGameObject = nullptr;

	if (Engine::Get_DIKeyState(DIK_F3) == Engine::KEYDOWN)
	{
		OBJINFO tMapObjectInfo = {};
		_tchar strMapObjectIndex[64] = { 0 };

		if (0 == m_iMapObjectType) // 키
		{
			_stprintf_s(strMapObjectIndex, _T("Key%d"), m_iMapObjectIndex);
			pGameObject = CKey::Create(m_pGraphicDev, m_pDefaultMapObject->m_pTransform->m_vInfo[INFO_POS]);
		}

		else if (1 == m_iMapObjectType) // 키 박스
		{
			_stprintf_s(strMapObjectIndex, _T("KeyBox%d"), m_iMapObjectIndex);
			pGameObject = CKeyBox::Create(m_pGraphicDev, m_pDefaultMapObject->m_pTransform->m_vInfo[INFO_POS]);
		}

		else if (2 == m_iMapObjectType) // 무브 박스
		{
			_stprintf_s(strMapObjectIndex, _T("MoveBox%d"), m_iMapObjectIndex);
			pGameObject = CMoveBox::Create(m_pGraphicDev, m_pDefaultMapObject->m_pTransform->m_vInfo[INFO_POS]);
		}

		else if (3 == m_iMapObjectType) // 포탈
		{
			_stprintf_s(strMapObjectIndex, _T("Portal%d"), m_iMapObjectIndex);
			pGameObject = CPortal::Create(m_pGraphicDev, m_pDefaultMapObject->m_pTransform->m_vInfo[INFO_POS]);
		}

		tMapObjectInfo.vObjPos = pGameObject->m_pTransform->m_vInfo[INFO_POS];
		tMapObjectInfo.iObjTypeNumber = m_iMapObjectType;

		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pStageLayer->Add_GameObject(strMapObjectIndex, pGameObject), );
		pGameObject->m_pTransform->m_bIsStatic = true;
		m_vecMapObjectInfo.push_back(tMapObjectInfo);
		++m_iMapObjectIndex;
	}
}

HRESULT CImguiUnit::SaveMapObject()
{
	HANDLE hFile = CreateFile(L"../Data/MapObjectPos.dat", GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD	dwByte = 0;

	for (auto& iter : m_vecMapObjectInfo)
		WriteFile(hFile, &iter, sizeof(OBJINFO), &dwByte, nullptr);

	CloseHandle(hFile);
	return S_OK;
}

HRESULT CImguiUnit::LoadMapObject()
{
	m_vecMonsterInfo.clear();

	HANDLE hFile = CreateFile(L"../Data/MapObjectPos.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD	dwByte = 0;
	OBJINFO vMapObjectInfo = {};

	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	NULL_CHECK_RETURN(pStageLayer, E_FAIL);

	CGameObject* pGameObject = nullptr;

	while (true)
	{
		ReadFile(hFile, &vMapObjectInfo, sizeof(OBJINFO), &dwByte, nullptr);
		if (dwByte == 0)
			break;
		m_vecMapObjectInfo.push_back(vMapObjectInfo);
	}
	CloseHandle(hFile);

	for (auto& iter : m_vecMapObjectInfo)
	{
		_tchar strMapObjectIndex[64] = { 0 };

		// 키
		if (0 == iter.iObjTypeNumber)
		{
			_stprintf_s(strMapObjectIndex, _T("Key%d"), m_iMapObjectIndex);
			pGameObject = CKey::Create(m_pGraphicDev, iter.vObjPos);
		}

		// 키박스
		else if (1 == iter.iObjTypeNumber)
		{
			_stprintf_s(strMapObjectIndex, _T("KeyBox%d"), m_iMapObjectIndex);
			pGameObject = CKeyBox::Create(m_pGraphicDev, iter.vObjPos);
		}

		// 무브 박스
		else if (2 == iter.iObjTypeNumber)
		{
			_stprintf_s(strMapObjectIndex, _T("MoveBox%d"), m_iMapObjectIndex);
			pGameObject = CMoveBox::Create(m_pGraphicDev, iter.vObjPos);
		}

		// 포탈
		else if (3 == iter.iObjTypeNumber)
		{
			_stprintf_s(strMapObjectIndex, _T("Portal%d"), m_iMapObjectIndex);
			pGameObject = CPortal::Create(m_pGraphicDev, iter.vObjPos);
		}

		NULL_CHECK_RETURN(pGameObject, E_FAIL);
		FAILED_CHECK_RETURN(pStageLayer->Add_GameObject(strMapObjectIndex, pGameObject), E_FAIL);

		pGameObject->m_pTransform->m_bIsStatic = false;
		++m_iMapObjectIndex;
	}

	return S_OK;
}

CImguiUnit * CImguiUnit::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CImguiUnit* pInstance = new CImguiUnit(pGraphicDev);

	if (nullptr == pInstance)
		return nullptr;

	return pInstance;
}
