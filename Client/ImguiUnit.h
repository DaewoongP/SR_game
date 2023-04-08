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
	CGameObject*	m_pDefaultMonster;		// 디폴트 몬스터 관련
	vector<OBJINFO> m_vecMonsterInfo;		// 몬스터 저장을 위한 공간
	bool m_bMonsterON;						// 몬스터 on / off
	int m_iMonsterType;						// 몬스터 종류
	int m_iMonsterindex;					// 몬스터 개수	
		
	// 맵 오브젝트 관련
	CGameObject*	m_pDefaultMapObject;	// 디폴트 맵 오브젝트 관련
	vector<OBJINFO> m_vecMapObjectInfo;		// 맵 오브젝트 저장을 위한 공간
	bool m_bMapObjectON;					// 맵 오브젝트 on / off
	int m_iMapObjectType;					// 맵 오브젝트 종류
	int m_iMapObjectIndex;					// 맵 오브젝트 개수

private:
	// 몬스터 메뉴
	HRESULT MonsterMenu();
	CGameObject* CreateDefaultMonster();
	void MonsterInstall();
	HRESULT SaveMonster();
	HRESULT LoadMonster();

	// 맵 오브젝트 메뉴
	HRESULT MapObjectMenu();
	CGameObject* CreateDefaultMapObject();
	void MapObjectInstall();
	HRESULT SaveMapObject();
	HRESULT LoadMapObject();

public:
	static	CImguiUnit* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};