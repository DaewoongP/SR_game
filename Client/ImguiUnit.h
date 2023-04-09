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

	HRESULT SaveMonster();
	HRESULT LoadMonster();
	HRESULT SaveMapObject();
	HRESULT LoadMapObject();

private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;

	// ���� ����
	CGameObject*	m_pDefaultMonster;		// ����Ʈ ���� ����
	vector<OBJINFO> m_vecMonsterInfo;		// ���� ������ ���� ����
	bool m_bMonsterON;						// ���� on / off
	int m_iMonsterType;						// ���� ����

	// �� ������Ʈ ����
	CGameObject*	m_pDefaultMapObject;	// ����Ʈ �� ������Ʈ ����
	vector<OBJINFO> m_vecMapObjectInfo;		// �� ������Ʈ ������ ���� ����
	bool m_bMapObjectON;					// �� ������Ʈ on / off
	int m_iMapObjectType;					// �� ������Ʈ ����

private:
	// ���� �޴�
	HRESULT MonsterMenu();
	void CreateDefaultMonster();
	void MonsterInstall();

	// �� ������Ʈ �޴�
	HRESULT MapObjectMenu();
	void CreateDefaultMapObject();
	void MapObjectInstall();

public:
	static	CImguiUnit* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};