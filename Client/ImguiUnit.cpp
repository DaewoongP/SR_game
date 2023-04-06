#include "stdafx.h"
#include "ImguiUnit.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "Export_Function.h"

CImguiUnit::CImguiUnit(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev),
	m_bMonsterON(false),
	m_iMonsterType(0)
{
}

CImguiUnit::~CImguiUnit()
{
}

_int CImguiUnit::Update_Imgui_Unit()
{
	MonsterMenu(m_pGraphicDev);

	return S_OK;
}

void CImguiUnit::Release()
{
}

HRESULT CImguiUnit::MonsterMenu(LPDIRECT3DDEVICE9 m_pGraphicDev)
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

CImguiUnit * CImguiUnit::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CImguiUnit* pInstance = new CImguiUnit(pGraphicDev);

	if (nullptr == pInstance)
		return nullptr;

	return pInstance;
}
