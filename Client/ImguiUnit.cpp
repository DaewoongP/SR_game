#include "stdafx.h"
#include "ImguiUnit.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "Export_Function.h"

#include "DefaultMonster.h"
#include "Pig.h"

CImguiUnit::CImguiUnit(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev),
	m_bMonsterON(false), m_bMapObjectON(false),
	m_iMonsterType(0), m_iMapObjectType(0), m_iMonsterindex(0)
{
	m_pDefaultMonster = nullptr;
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
			m_pDefaultMonster = CreateDefaultMonster();

		// 체크 박스 활성화 시 몬스터 설치 부분
		if (m_bMonsterON && nullptr != m_pDefaultMonster)
			MonsterInstall();

		// 체크 박스가 꺼졌을 때 디폴트 몬스터 사망 처리
		if (!m_bMonsterON && nullptr != m_pDefaultMonster)
		{
			m_pDefaultMonster->m_bDead = true;
			m_pDefaultMonster = nullptr;
		}

		ImGui::TreePop();
	}

	return S_OK;
}

CGameObject * CImguiUnit::CreateDefaultMonster()
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	NULL_CHECK_RETURN(pStageLayer, nullptr);

	CGameObject* pGameObject = nullptr;

	pGameObject = CDefaultMonster::Create(m_pGraphicDev);

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
		_tchar strCubeIndex[64] = { 0 };

		if (0 == m_iMonsterType) // 돼지
		{
			_stprintf_s(strCubeIndex, _T("Pig%d"), m_iMonsterindex);
			pGameObject = CPig::Create(m_pGraphicDev);
		}

		pGameObject->m_pTransform->m_vInfo[INFO_POS] = m_pDefaultMonster->m_pTransform->m_vInfo[INFO_POS];

		NULL_CHECK_RETURN(pGameObject, );
		FAILED_CHECK_RETURN(pStageLayer->Add_GameObject(strCubeIndex, pGameObject), );
		pGameObject->m_pTransform->m_bIsStatic = false;

		++m_iMonsterindex;
	}
}

HRESULT CImguiUnit::MapObjectMenu()
{
	if (ImGui::TreeNode("Map Object"))
	{
		// 맵 오브젝트 설치 체크 박스
		ImGui::Checkbox("Map Object Placed", &m_bMapObjectON);

		// 맵 오브젝트 종류 선택 콤보 박스
		const char* items[] = { "KEY", "PADLICK BOX", "BOX" };
		ImGui::Combo("Map Object Type", &m_iMapObjectType, items, IM_ARRAYSIZE(items));

		ImGui::TreePop();
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
