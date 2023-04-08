#pragma once

#include "Include.h"

class CImguiStage
{
public:
	explicit CImguiStage(LPDIRECT3DDEVICE9 pGraphicDev);
	~CImguiStage();

public:
	_int Update_Imgui_Stage();
	void Release();

private:
	LPDIRECT3DDEVICE9 m_pGraphicDev;

	// 그리드 관련
	vector<CGameObject*> m_vecGroundGrid;	// 그리드 모음
	bool m_bGridON;							// 그리드 on / off
	bool m_bGridCreate;						// 최초 그리드 생성

	// 큐브 관련
	CGameObject* m_pDefaultCube;			// 디폴트 큐브
	vector<OBJINFO> m_vecCubeInfo;			// 큐브 모음(저장 용도)

	bool m_bCubePlaced;						// 디폴트 큐브 생성 
	int m_iCubeTextureNumber;				// 큐브 스타일 번호
	int m_iCubeIndex;						// 설치된 큐브 개수

private:
	// 그리드 메뉴
	HRESULT GridMeun();
	HRESULT GroundGridON();

	// 큐브 메뉴
	HRESULT CubeMeun();
	CGameObject* CreateDefaultCube();
	void	CubeInstall();

	HRESULT SaveCube();
	HRESULT LoadCube();
	
public:
	static	CImguiStage* Create(LPDIRECT3DDEVICE9 pGraphicDev);
};

