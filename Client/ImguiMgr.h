#pragma once
#include"..\Engine\Define.h"
#include "Include.h"

class CImguiStage;
class CImguiUnit;
class CImguiBG;

class CImguiMgr
{
	DECLARE_SINGLETON(CImguiMgr)
		
private:
	explicit CImguiMgr();
	~CImguiMgr();

public:
	HRESULT Ready_Imgui(LPDIRECT3DDEVICE9 m_pGraphicDev);
	HRESULT Update_Imgui(LPDIRECT3DDEVICE9 m_pGraphicDev);
	void	Release();

	CImguiStage* Get_Stage() { return m_pImguiStage; }
	CImguiUnit*	Get_Unit() { return m_pImguiUnit; }
private:
	CImguiStage* m_pImguiStage;		// 스테이지 툴
	CImguiUnit*  m_pImguiUnit;		// 유닛 툴
	CImguiBG*	 m_pImguiBG; //배경툴


	CGameObject* m_pToodee;			// 투디가 있는지 확인
	_bool		 m_bOnceLoad;		// 최초 한 번 로드
	_bool		 m_bDeleteAll;		// 진짜 다 지울껀지 물어보는 변수
	_int		 m_iStageNumber;	// 스테이지 고르는 변수

	// window 창 onoff 관련 변수
	bool m_bStageTool; // 스테이지 onoff
	bool m_bUnitTool;  // 유닛 onoff
	bool m_bBGTool;
};