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

	// 몬스터 관련
	bool m_bMonsterON;					// 몬스터 on / off

	int m_iMonsterType;					// 몬스터 종류

private:
	HRESULT MonsterMenu(LPDIRECT3DDEVICE9 m_pGraphicDev);

public:
	static	CImguiUnit* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};