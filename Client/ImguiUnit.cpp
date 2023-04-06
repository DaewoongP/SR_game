#include "stdafx.h"
#include "ImguiUnit.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "Export_Function.h"

CImguiUnit::CImguiUnit(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev),
	m_bMonsterON(false), m_bMapObjectON(false),
	m_iMonsterType(0), m_iMapObjectType(0)
{
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
		// ���� ��ġ üũ �ڽ�
		ImGui::Checkbox("Monster Placed", &m_bMonsterON);

		// ���� ���� ���� �޺� �ڽ�
		const char* items[] = { "PIG", "BAT" };
		ImGui::Combo("Monster Type", &m_iMonsterType, items, IM_ARRAYSIZE(items));

		ImGui::TreePop();
	}

	return S_OK;
}

HRESULT CImguiUnit::MapObjectMenu()
{
	if (ImGui::TreeNode("Map Object"))
	{
		// �� ������Ʈ ��ġ üũ �ڽ�
		ImGui::Checkbox("Map Object Placed", &m_bMapObjectON);

		// �� ������Ʈ ���� ���� �޺� �ڽ�
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
