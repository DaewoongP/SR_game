#include "stdafx.h"
#include "ImguiUnit.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

#include "DefaultUnit.h"
#include "Pig.h"
#include "Bat.h"
#include "Portal.h"
#include "Key.h"
#include "KeyCube.h"
#include "MoveCube.h"
#include "CrackCube.h"
#include "Spike.h"
#include "PinkCloud.h"
#include "Switch.h"
#include "SwitchCube.h"
#include "GravityCube.h"
#include "LightningCloud.h"
#include "PortalCube.h"
#include "LaserTurret.h"

CImguiUnit::CImguiUnit(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev),
	m_bMonsterON(false), m_bMapObjectON(false),
	m_iMonsterType(0), m_iMapObjectType(0), m_iPortalCubeCount(0), m_iLaserTurretCount(0),
	m_tPortalCubeDir(CD_UP), m_tLaserTurretDir(CD_LEFT)
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
		ImGui::Text("Install : F2");

		// 몬스터 설치 체크 박스
		ImGui::Checkbox("Monster Placed", &m_bMonsterON);

		// 몬스터 종류 선택 콤보 박스
		const char* items[] = { "PIG", "BAT" };
		ImGui::Combo("Monster Type", &m_iMonsterType, items, IM_ARRAYSIZE(items));

		// 체크 박스가 켜졌을 때 디폴트 몬스터 생성
		if (m_bMonsterON && nullptr == m_pDefaultMonster)
		{
			CreateDefaultMonster();

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
			FAILED_CHECK_RETURN(SaveMonster(m_iStageNumber), E_FAIL);

		// 로드 기능
		ImGui::SameLine();
		if (ImGui::Button("Monster Load"))
			FAILED_CHECK_RETURN(LoadMonster(m_iStageNumber), E_FAIL);

		ImGui::TreePop();
	}

	return S_OK;
}

void CImguiUnit::CreateDefaultMonster()
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	NULL_CHECK_RETURN(pStageLayer, );

	FAILED_CHECK_RETURN(FACTORY<CDefaultUnit>::Create(L"DefaultMonster", pStageLayer, _vec3{ 0.f, 0.f, 0.f }), );

	m_pDefaultMonster = Engine::Get_GameObject(L"Layer_GameLogic", L"DefaultMonster");
	dynamic_cast<CDefaultUnit*>(m_pDefaultMonster)->Set_DefaultIndex(CDefaultUnit::DMonster);

	return;
}

void CImguiUnit::MonsterInstall()
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	NULL_CHECK_RETURN(pStageLayer, );

	if (Engine::Get_DIKeyState(DIK_F2) == Engine::KEYDOWN)
	{
		OBJINFO tMonsterInfo = {};

		if (0 == m_iMonsterType) // 돼지
		{
			FAILED_CHECK_RETURN(FACTORY<CPig>::Create(L"Pig", pStageLayer,
				m_pDefaultMonster->m_pTransform->m_vInfo[INFO_POS]), );
		}

		else if (1 == m_iMonsterType) // 박쥐
		{
			FAILED_CHECK_RETURN(FACTORY<CBat>::Create(L"Bat", pStageLayer,
				m_pDefaultMonster->m_pTransform->m_vInfo[INFO_POS]), );
		}

		tMonsterInfo.vObjPos = m_pDefaultMonster->m_pTransform->m_vInfo[INFO_POS];
		tMonsterInfo.iObjTypeNumber = m_iMonsterType;

		m_vecMonsterInfo.push_back(tMonsterInfo);
	}
}

HRESULT CImguiUnit::SaveMonster(_int iStageNumber)
{
	TCHAR dataFile[128] = { 0 };
	_stprintf_s(dataFile, _T("../Data/MonsterPos%d.dat"), (iStageNumber + 1));

	HANDLE hFile = CreateFile(dataFile, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD	dwByte = 0;

	for (auto& iter : m_vecMonsterInfo)
		WriteFile(hFile, &iter, sizeof(OBJINFO), &dwByte, nullptr);

	CloseHandle(hFile);
	return S_OK;
}

HRESULT CImguiUnit::LoadMonster(_int iStageNumber, CScene* pScene)
{
	m_vecMonsterInfo.clear();

	TCHAR dataFile[128] = { 0 };
	_stprintf_s(dataFile, _T("../Data/MonsterPos%d.dat"), (iStageNumber + 1));

	HANDLE hFile = CreateFile(dataFile, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD	dwByte = 0;
	OBJINFO vMonsterInfo = {};

	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	if (pStageLayer == nullptr)
		pStageLayer = pScene->Get_Layer(L"Layer_GameLogic");
	else
		return E_FAIL;

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
			FAILED_CHECK_RETURN(FACTORY<CPig>::Create(L"Pig", pStageLayer, iter.vObjPos), E_FAIL);
		}

		// 박쥐
		else if (1 == iter.iObjTypeNumber)
		{
			FAILED_CHECK_RETURN(FACTORY<CBat>::Create(L"Bat", pStageLayer, iter.vObjPos), E_FAIL);
		}
	}

	return S_OK;
}

HRESULT CImguiUnit::MapObjectMenu()
{
	if (ImGui::TreeNode("Map Object"))
	{
		ImGui::Text("Install : F3");

		// 맵 오브젝트 설치 체크 박스
		ImGui::Checkbox("Map Object Placed", &m_bMapObjectON);

		// 맵 오브젝트 종류 선택 콤보 박스
		const char* items[] = { "KEY", "KEY CUBE", "MOVE CUBE", "PORTAL", "CRACK CUBE", "SPIKE", "PINKCLOUD",
			"SWITCH", "SWITCH CUBE", "GRAVITY CUBE", "LIGHTNING CLOUD", "PORTAL CUBE", "LASER TURRET" };
		ImGui::Combo("Map Object Type", &m_iMapObjectType, items, IM_ARRAYSIZE(items));

		// 포탈 큐브 방향 설정
		if (11 == m_iMapObjectType)
		{
			if (ImGui::Button("LEFT"))
				m_tPortalCubeDir = CD_LEFT;

			ImGui::SameLine();
			if (ImGui::Button("RIGHT"))
				m_tPortalCubeDir = CD_RIGHT;

			ImGui::SameLine();
			if (ImGui::Button("UP"))
				m_tPortalCubeDir = CD_UP;

			ImGui::SameLine();
			if (ImGui::Button("DOWN"))
				m_tPortalCubeDir = CD_DOWN;
		}

		// 레이저 터렛 방향 설정
		if (12 == m_iMapObjectType)
		{
			if (ImGui::Button("LEFT"))
				m_tLaserTurretDir = CD_LEFT;

			ImGui::SameLine();
			if (ImGui::Button("RIGHT"))
				m_tLaserTurretDir = CD_RIGHT;
		}

		// 체크 박스가 켜졌을 때 디폴트 맵 오브젝트 생성
		if (m_bMapObjectON && nullptr == m_pDefaultMapObject)
		{
			CreateDefaultMapObject();

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
			FAILED_CHECK_RETURN(SaveMapObject(m_iStageNumber), E_FAIL);

		// 로드 기능
		ImGui::SameLine();
		if (ImGui::Button("MapObject Load"))
			FAILED_CHECK_RETURN(LoadMapObject(m_iStageNumber), E_FAIL);

		ImGui::TreePop();
	}

	return S_OK;
}

void CImguiUnit::CreateDefaultMapObject()
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	NULL_CHECK_RETURN(pStageLayer, );

	FAILED_CHECK_RETURN(FACTORY<CDefaultUnit>::Create(L"DefaultMapObject", pStageLayer, _vec3{ 0.f, 0.f, 0.f }), );

	m_pDefaultMapObject = Engine::Get_GameObject(L"Layer_GameLogic", L"DefaultMapObject");
	dynamic_cast<CDefaultUnit*>(m_pDefaultMapObject)->Set_DefaultIndex(CDefaultUnit::DMapObject);

	return;
}

void CImguiUnit::MapObjectInstall()
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	if (nullptr == pStageLayer)
		return;

	if (Engine::Get_DIKeyState(DIK_F3) == Engine::KEYDOWN)
	{
		OBJINFO tMapObjectInfo = {};

		if (0 == m_iMapObjectType) // 키
		{
			FAILED_CHECK_RETURN(FACTORY<CKey>::Create(L"Key", pStageLayer,
				m_pDefaultMapObject->m_pTransform->m_vInfo[INFO_POS]), );
		}

		else if (1 == m_iMapObjectType) // 키 큐브
		{
			FAILED_CHECK_RETURN(FACTORY<CKeyCube>::Create(L"KeyCube", pStageLayer,
				m_pDefaultMapObject->m_pTransform->m_vInfo[INFO_POS]), );
		}

		else if (2 == m_iMapObjectType) // 무브 큐브
		{
			FAILED_CHECK_RETURN(FACTORY<CMoveCube>::Create(L"MoveCube", pStageLayer,
				m_pDefaultMapObject->m_pTransform->m_vInfo[INFO_POS]), );
		}

		else if (3 == m_iMapObjectType) // 포탈
		{
			FAILED_CHECK_RETURN(FACTORY<CPortal>::Create(L"Portal", pStageLayer,
				m_pDefaultMapObject->m_pTransform->m_vInfo[INFO_POS]), );
		}

		else if (4 == m_iMapObjectType) // 밟으면 없어지는 큐브
		{
			FAILED_CHECK_RETURN(FACTORY<CCrackCube>::Create(L"CrackCube", pStageLayer,
				m_pDefaultMapObject->m_pTransform->m_vInfo[INFO_POS]), );
		}

		else if (5 == m_iMapObjectType) // 스파이크
		{
			FAILED_CHECK_RETURN(FACTORY<CSpike>::Create(L"Spike", pStageLayer,
				m_pDefaultMapObject->m_pTransform->m_vInfo[INFO_POS]), );
		}

		else if (6 == m_iMapObjectType) // 분홍구름
		{
			FAILED_CHECK_RETURN(FACTORY<CPinkCloud>::Create(L"PinkCloud", pStageLayer,
				m_pDefaultMapObject->m_pTransform->m_vInfo[INFO_POS]), );
		}

		else if (7 == m_iMapObjectType) // 스위치
		{
			FAILED_CHECK_RETURN(FACTORY<CSwitch>::Create(L"Swtich", pStageLayer,
				m_pDefaultMapObject->m_pTransform->m_vInfo[INFO_POS]), );
		}

		else if (8 == m_iMapObjectType) // 스위치 큐브
		{
			FAILED_CHECK_RETURN(FACTORY<CSwitchCube>::Create(L"SwtichCube", pStageLayer,
				m_pDefaultMapObject->m_pTransform->m_vInfo[INFO_POS]), );
		}

		else if (9 == m_iMapObjectType) // 중력 큐브
		{
			FAILED_CHECK_RETURN(FACTORY<CGravityCube>::Create(L"GravityCube", pStageLayer,
				m_pDefaultMapObject->m_pTransform->m_vInfo[INFO_POS]), );
		}

		else if (10 == m_iMapObjectType) // 번개 구름
		{
			FAILED_CHECK_RETURN(FACTORY<CLightningCloud>::Create(L"LightningCloud", pStageLayer,
				m_pDefaultMapObject->m_pTransform->m_vInfo[INFO_POS]), );
		}

		else if (11 == m_iMapObjectType && (2 >  m_iPortalCubeCount)) // 포탈 큐브, 맵에 최대로 설치 가능한 개수는 2개
		{
			FAILED_CHECK_RETURN(FACTORY<CPortalCube>::Create(L"PortalCube", pStageLayer,
				m_pDefaultMapObject->m_pTransform->m_vInfo[INFO_POS], (_int)m_tPortalCubeDir), );

			m_vecPortalCubeDir.push_back(m_tPortalCubeDir);
			++m_iPortalCubeCount;
		}

		else if (12 == m_iMapObjectType) // 레이저 터렛
		{
			FAILED_CHECK_RETURN(FACTORY<CLaserTurret>::Create(L"LaserTurret", pStageLayer,
				m_pDefaultMapObject->m_pTransform->m_vInfo[INFO_POS], (_int)(m_tLaserTurretDir - 2)), );

			m_vecLaserTurretDir.push_back(m_tLaserTurretDir);
		}

		tMapObjectInfo.vObjPos = m_pDefaultMapObject->m_pTransform->m_vInfo[INFO_POS];
		tMapObjectInfo.iObjTypeNumber = m_iMapObjectType;

		m_vecMapObjectInfo.push_back(tMapObjectInfo);
	}
}

HRESULT CImguiUnit::SaveMapObject(_int iStageNumber)
{
	TCHAR dataFile[128] = { 0 };
	_stprintf_s(dataFile, _T("../Data/MapObjectPos%d.dat"), (iStageNumber + 1));

	HANDLE hFile = CreateFile(dataFile, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD    dwByte = 0;

	for (auto& iter : m_vecMapObjectInfo)
		WriteFile(hFile, &iter, sizeof(OBJINFO), &dwByte, nullptr);

	CloseHandle(hFile);

	TCHAR dataFile2[128] = { 0 };
	_stprintf_s(dataFile2, _T("../Data/PortalCubeDir%d.dat"), (iStageNumber + 1));

	HANDLE hFile2 = CreateFile(dataFile2, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile2)
		return E_FAIL;

	DWORD dwByte1 = 0;

	for (auto&iter : m_vecPortalCubeDir)
		WriteFile(hFile, &iter, sizeof(_int), &dwByte1, nullptr);

	CloseHandle(hFile2);

	TCHAR dataFile3[128] = { 0 };
	_stprintf_s(dataFile3, _T("../Data/LaserTurretPos%d.dat"), (iStageNumber + 1));

	HANDLE hFile3 = CreateFile(dataFile3, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile3)
		return E_FAIL;

	DWORD dwByte2 = 0;

	for (auto& iter : m_vecLaserTurretDir)
		WriteFile(hFile, &iter, sizeof(OBJINFO), &dwByte2, nullptr);

	CloseHandle(hFile3);

	return S_OK;
}
HRESULT CImguiUnit::LoadMapObject(_int iStageNumber, CScene* pScene)
{
	m_vecMapObjectInfo.clear();
	m_vecPortalCubeDir.clear();
	m_vecLaserTurretDir.clear();

	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	if (pStageLayer == nullptr)
		pStageLayer = pScene->Get_Layer(L"Layer_GameLogic");
	else
		return E_FAIL;

	TCHAR dataFile[128] = { 0 };
	_stprintf_s(dataFile, _T("../Data/MapObjectPos%d.dat"), (iStageNumber + 1));

	HANDLE hFile = CreateFile(dataFile, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD    dwByte = 0;
	OBJINFO vMapObjectInfo = {};

	while (true)
	{
		ReadFile(hFile, &vMapObjectInfo, sizeof(OBJINFO), &dwByte, nullptr);
		if (dwByte == 0)
			break;
		m_vecMapObjectInfo.push_back(vMapObjectInfo);
	}

	CloseHandle(hFile);

	TCHAR dataFile2[128] = { 0 };
	_stprintf_s(dataFile2, _T("../Data/PortalCubeDir%d.dat"), (iStageNumber + 1));

	HANDLE hFile2 = CreateFile(dataFile2, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile2)
		return E_FAIL;

	DWORD dwByte1 = 0;

	int vPortalCubeInfo = {};

	while (true)
	{
		ReadFile(hFile2, &vPortalCubeInfo, sizeof(_int), &dwByte1, nullptr);
		if (dwByte1 == 0)
			break;
		m_vecPortalCubeDir.push_back(vPortalCubeInfo);
	}

	CloseHandle(hFile2);

	TCHAR dataFile3[128] = { 0 };
	_stprintf_s(dataFile3, _T("../Data/LaserTurretPos%d.dat"), (iStageNumber + 1));

	HANDLE hFile3 = CreateFile(dataFile3, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile2)
		return E_FAIL;

	DWORD dwByte2 = 0;

	int vLaserTurretInfo = {};

	while (true)
	{
		ReadFile(hFile2, &vLaserTurretInfo, sizeof(_int), &dwByte2, nullptr);
		if (dwByte2 == 0)
			break;
		m_vecLaserTurretDir.push_back(vLaserTurretInfo);
	}

	CloseHandle(hFile3);

	for (auto& iter : m_vecMapObjectInfo)
	{
		if (0 == iter.iObjTypeNumber) // 키
		{
			FAILED_CHECK_RETURN(FACTORY<CKey>::Create(L"Key", pStageLayer, iter.vObjPos), E_FAIL);
		}

		else if (1 == iter.iObjTypeNumber) // 키 큐브
		{
			FAILED_CHECK_RETURN(FACTORY<CKeyCube>::Create(L"KeyCube", pStageLayer, iter.vObjPos), E_FAIL);
		}

		else if (2 == iter.iObjTypeNumber) // 무브 큐브
		{
			FAILED_CHECK_RETURN(FACTORY<CMoveCube>::Create(L"MoveCube", pStageLayer, iter.vObjPos), E_FAIL);
		}

		else if (3 == iter.iObjTypeNumber) // 포탈
		{
			FAILED_CHECK_RETURN(FACTORY<CPortal>::Create(L"Portal", pStageLayer, iter.vObjPos), E_FAIL);
		}

		else if (4 == iter.iObjTypeNumber) // 밟으면 없어지는 큐브
		{
			FAILED_CHECK_RETURN(FACTORY<CCrackCube>::Create(L"CrackCube", pStageLayer, iter.vObjPos), E_FAIL);
		}

		else if (5 == iter.iObjTypeNumber) // 스파이크
		{
			FAILED_CHECK_RETURN(FACTORY<CSpike>::Create(L"Spike", pStageLayer, iter.vObjPos), E_FAIL);
		}

		else if (6 == iter.iObjTypeNumber) // 분홍구름
		{
			FAILED_CHECK_RETURN(FACTORY<CPinkCloud>::Create(L"PinkCloud", pStageLayer, iter.vObjPos), E_FAIL);
		}

		else if (7 == iter.iObjTypeNumber) // 스위치
		{
			FAILED_CHECK_RETURN(FACTORY<CSwitch>::Create(L"Switch", pStageLayer, iter.vObjPos), E_FAIL);
		}

		else if (8 == iter.iObjTypeNumber) // 스위치 큐브
		{
			FAILED_CHECK_RETURN(FACTORY<CSwitchCube>::Create(L"SwitchCube", pStageLayer, iter.vObjPos), E_FAIL);
		}

		else if (9 == iter.iObjTypeNumber) // 중력 큐브
		{
			FAILED_CHECK_RETURN(FACTORY<CGravityCube>::Create(L"GravityCube", pStageLayer, iter.vObjPos), E_FAIL);
		}

		else if (10 == iter.iObjTypeNumber) // 번개 구름
		{
			FAILED_CHECK_RETURN(FACTORY<CLightningCloud>::Create(L"LightningCloud", pStageLayer, iter.vObjPos), E_FAIL);
		}

		else if (11 == iter.iObjTypeNumber && 2 > m_iPortalCubeCount) // 포탈 큐브
		{
			FAILED_CHECK_RETURN(FACTORY<CPortalCube>::Create(L"PortalCube", pStageLayer, iter.vObjPos,
				(_int)m_vecPortalCubeDir.at(m_iPortalCubeCount)), E_FAIL);
			++m_iPortalCubeCount;
		}

		else if (12 == iter.iObjTypeNumber) // 레이저 터렛
		{
			FAILED_CHECK_RETURN(FACTORY<CLaserTurret>::Create(L"LaserTurret", pStageLayer, iter.vObjPos,
				(_int)(m_vecLaserTurretDir.at(m_iLaserTurretCount) - 2)), E_FAIL);
			++m_iLaserTurretCount;
		}
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
