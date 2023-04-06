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

	// 맵 오브젝트 관련
	bool m_bMapObjectON;				// 맵 오브젝트 omn / off
	int m_iMapObjectType;				// 맵 오브젝트 종류

private:
	// 몬스터 메뉴
	HRESULT MonsterMenu();

	// 맵 오브젝트 메뉴
	HRESULT MapObjectMenu();

public:
	static	CImguiUnit* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};