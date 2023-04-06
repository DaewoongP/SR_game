#pragma once
#include"..\Engine\Define.h"
#include "Include.h"
class CImguiStage;

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

	CImguiStage* m_pImguiStage; // �������� ��

	// window â onoff ���� ����
	bool m_bStageTool;
};