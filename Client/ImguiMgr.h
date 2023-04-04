#pragma once
#include"..\Engine\Define.h"

class CImguiMgr
{
private:
	explicit CImguiMgr();
	~CImguiMgr();

public:
	HRESULT Update_Imgui(LPDIRECT3DDEVICE9 m_pGraphicDev);
	void Release();

private:
	typedef struct CubeInfo
	{
		_vec3	vCubePos;
		int		iCubeTextureNumber;
	}CUBEINFO;

private:
	HRESULT SaveData(vector<CUBEINFO>& vecCubePos);
	HRESULT LoadData(LPDIRECT3DDEVICE9 m_pGraphicDev, vector<CUBEINFO>& vecCubePos, int iCubeIndex);
	CGameObject* CreateDefaultCube(LPDIRECT3DDEVICE9 m_pGraphicDev);
	void	CubeInstall(CGameObject* pDefaultCube, LPDIRECT3DDEVICE9 m_pGraphicDev, vector<CUBEINFO>& vecCubePos, int iCubeIndex);
	HRESULT GroundGridON(LPDIRECT3DDEVICE9 m_pGraphicDev, vector<CGameObject*>& vecGrid);
};