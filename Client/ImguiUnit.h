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

	// ���� ������ ���� ����ü
	typedef struct MonsterInfo
	{
		_vec3	vMonsterPos;					// ���� ��ġ
		int		iMonsterTypeNumber;				// ���� ���� ��ȣ
	}MONSTERINFO;

	// ���� ����
	CGameObject*	m_pDefaultMonster;		// ����Ʈ ���� ����
	vector<MONSTERINFO> m_vecMonsterInfo;	// ���� ������ ���� ����
	bool m_bMonsterON;						// ���� on / off
	int m_iMonsterType;						// ���� ����
	int m_iMonsterindex;					// ���� ����	
		
	// �� ������Ʈ ����
	bool m_bMapObjectON;				// �� ������Ʈ omn / off
	int m_iMapObjectType;				// �� ������Ʈ ����

private:
	// ���� �޴�
	HRESULT MonsterMenu();
	CGameObject* CreateDefaultMonster();
	void MonsterInstall();

	// �� ������Ʈ �޴�
	HRESULT MapObjectMenu();

public:
	static	CImguiUnit* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};