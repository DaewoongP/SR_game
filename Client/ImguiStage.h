#pragma once
#include "Include.h"

class CImguiStage
{
public:
	explicit CImguiStage(LPDIRECT3DDEVICE9 pGraphicDev);
	~CImguiStage();

public:
	void	Set_StageNumber(_int _iNumber) { m_iStageNumber = _iNumber; }
	vector<OBJINFO>* Get_GridVector() { return &m_vecInstallGrid; }
	vector<OBJINFO>* Get_CubeVector() { return &m_vecCubeInfo; }
	vector<OBJINFO>* get_InvGridVector() { return &m_vecInvGridInfo; }
public:
	_int Update_Imgui_Stage();
	void Release();
	
	HRESULT SaveGrid(_int iStageNumber);
	HRESULT LoadGrid(_int iStageNumber, CScene* pScene);
	HRESULT SaveCube(_int iStageNumber);
	HRESULT LoadCube(_int iStageNumber, CScene* pScene);

	HRESULT	Undo(_int iStageNumber);
private:
	// 그리드 메뉴
	HRESULT GridMenu();
	void	GroundGridON();
	void	CreateDefaultGrid();
	void	GridInstall();

	void Scale();

	// 큐브 메뉴
	HRESULT CubeMenu();
	void	CreateDefaultCube();
	void	CubeInstall();

public:
	static bool m_bGridON;					// 그라운드 그리드 on / off

private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;

	// 그리드 관련
	CGameObject* m_pDefaultGrid;			// 디폴트 그리드 관련
	vector<CGameObject*> m_vecGroundGrid;	// 그라운드 그리드 모음
	vector<OBJINFO> m_vecInstallGrid;		// 설치된 그리드 모음(저장 용도)
	vector<CGameObject*> m_vecGameObject;	// 지우기용 오브젝트 벡터


	bool m_bGridCreate;						// 최초 그라운드 그리드 생성
	bool m_bDefaultGridCreate;				// 디폴트 그리드 생성

	// 큐브 관련
	CGameObject* m_pDefaultCube;			// 디폴트 큐브
	vector<OBJINFO> m_vecCubeInfo;			// 큐브 모음(저장 용도)

	bool m_bCubePlaced;						// 디폴트 큐브 생성 
	int m_iCubeTextureNumber;				// 큐브 스타일 번호

	_int m_iStageNumber;					// 스테이지 저장용 변수
	
	//invisiblegrid용
	CGameObject* m_pInvGird;			// 디폴트 큐브
	vector<OBJINFO> m_vecInvGridInfo;

public:
	static	CImguiStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	void MakeGameObject(CLayer* pLayer, const _tchar * pObjTag);
	void MakeGameObject(CLayer* pLayer, const _tchar * pObjTag, _int iNum);
};