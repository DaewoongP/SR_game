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

	// 몬스터 관련
	CGameObject*	m_pDefaultMonster;		// 디폴트 몬스터 관련
	vector<OBJINFO> m_vecMonsterInfo;		// 몬스터 저장을 위한 공간
	bool m_bMonsterON;						// 몬스터 on / off
	int m_iMonsterType;						// 몬스터 종류
	vector<CGameObject*> m_vecGameObject;	// 지우기용 벡터

	// 맵 오브젝트 관련
	CGameObject*	m_pDefaultMapObject;	// 디폴트 맵 오브젝트 관련
	vector<OBJINFO> m_vecMapObjectInfo;		// 맵 오브젝트 저장을 위한 공간
	bool m_bMapObjectON;					// 맵 오브젝트 on / off
	int m_iMapObjectType;					// 맵 오브젝트 종류

	// 맵 오브젝트 중 포탈 큐브 관련
	vector<int> m_vecPortalCubeDir;			// 포탈 큐브 방향 저장용
	int m_iPortalCubeCount;					// 포탈 큐브 개수
	CUBE_DIR	m_tPortalCubeDir;			// 포탈 큐브 방향

	// 맵 오브젝트 중 레이저 터렛 관련
	vector<_int> m_vecLaserTurretDir;		// 레이저 터렛 방향 저장용
	CUBE_DIR	m_tLaserTurretDir;			// 레이저 터렛 방향
	int			m_iLaserTurretCount;		// 레이저 터렛 개수

	// 스테이지 저장용 변수
	_int m_iStageNumber;

private:
	// 몬스터 메뉴
	HRESULT MonsterMenu();
	void CreateDefaultMonster();
	void MonsterInstall();

	// 맵 오브젝트 메뉴
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
