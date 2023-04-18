#pragma once

#include "Include.h"

class CImguiUnit
{
public:
	explicit CImguiUnit(LPDIRECT3DDEVICE9 pGraphicDev);
	~CImguiUnit();

public:
	void	Set_StageNumber(_int _iNumber) { m_iStageNumber = _iNumber; }

public:
	_int Update_Imgui_Unit();
	void Release();

	HRESULT SaveMonster(_int iStageNumber);
	HRESULT LoadMonster(_int iStageNumber, CScene* pScene = nullptr);
	HRESULT SaveMapObject(_int iStageNumber);
	HRESULT LoadMapObject(_int iStageNumber, CScene* pScene = nullptr);

	HRESULT	Undo(_int iStageNumber);
private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;

	// ���� ����
	CGameObject*	m_pDefaultMonster;		// ����Ʈ ���� ����
	vector<OBJINFO> m_vecMonsterInfo;		// ���� ������ ���� ����
	bool m_bMonsterON;						// ���� on / off
	int m_iMonsterType;						// ���� ����
	vector<CGameObject*> m_vecGameObject;	// ������ ����

	// �� ������Ʈ ����
	CGameObject*	m_pDefaultMapObject;	// ����Ʈ �� ������Ʈ ����
	vector<OBJINFO> m_vecMapObjectInfo;		// �� ������Ʈ ������ ���� ����
	bool m_bMapObjectON;					// �� ������Ʈ on / off
	int m_iMapObjectType;					// �� ������Ʈ ����

	// �� ������Ʈ �� ��Ż ť�� ����
	vector<int> m_vecPortalCubeDir;			// ��Ż ť�� ���� �����
	int m_iPortalCubeCount;					// ��Ż ť�� ����
	CUBE_DIR	m_tPortalCubeDir;			// ��Ż ť�� ����

	// �� ������Ʈ �� ������ �ͷ� ����
	vector<_int> m_vecLaserTurretDir;		// ������ �ͷ� ���� �����
	CUBE_DIR	m_tLaserTurretDir;			// ������ �ͷ� ����
	int			m_iLaserTurretCount;		// ������ �ͷ� ����

	// �������� ����� ����
	_int m_iStageNumber;

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

private:
	template<typename T>
	void MakeMonster(CLayer* pLayer, const _tchar * pObjTag);
	template<typename T>
	void MakeGameObject(CLayer* pLayer, const _tchar * pObjTag);
	template<typename T>
	void MakeGameObjectTypeNum(CLayer* pLayer, const _tchar * pObjTag, _int iNum);
};
