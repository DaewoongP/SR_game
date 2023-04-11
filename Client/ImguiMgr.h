#pragma once
#include"..\Engine\Define.h"
#include "Include.h"

class CImguiStage;
class CImguiUnit;

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

private:
	CImguiStage* m_pImguiStage;		// 스테이지 툴
	CImguiUnit*  m_pImguiUnit;		// 유닛 툴

	CGameObject* m_pToodee;			// 투디가 있는지 확인
	bool		 m_bOnceLoad;		// 최초 한 번 로드
	_int		 m_iStageNumber;	// 스테이지 고르는 변수

	// window 창 onoff 관련 변수
	bool m_bStageTool; // 스테이지 onoff
	bool m_bUnitTool;  // 유닛 onoff
};