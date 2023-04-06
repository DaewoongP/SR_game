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
	typedef struct CubeInfo
	{
		_vec3	vCubePos;
		int		iCubeTextureNumber;
	}CUBEINFO;

	CImguiStage* m_pImguiStage; // 스테이지 툴
	CImguiUnit*  m_pImguiUnit;  // 유닛 툴

	// window 창 onoff 관련 변수
	bool m_bStageTool; // 스테이지 onoff
	bool m_bUnitTool;  // 유닛 onoff
};