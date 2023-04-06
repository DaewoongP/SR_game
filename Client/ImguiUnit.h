#pragma once

#include "Include.h"

class CImguiUnit
{
public:
	explicit CImguiUnit(LPDIRECT3DDEVICE9 pGraphicDev);
	~CImguiUnit();

public:
	_int Update_Imgui_Unit();
	void Release();

private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;

	// ���� ����
	bool m_bMonsterON;					// ���� on / off

	int m_iMonsterType;					// ���� ����

private:
	HRESULT MonsterMenu(LPDIRECT3DDEVICE9 m_pGraphicDev);

public:
	static	CImguiUnit* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};