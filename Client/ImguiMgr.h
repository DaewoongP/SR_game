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
	HRESULT SaveData(vector<_vec3>& vecCubePos);
	HRESULT LoadData(LPDIRECT3DDEVICE9 m_pGraphicDev, vector<_vec3>& vecCubePos, int iCubeIndex);
	CGameObject* CreateDefaultCube(LPDIRECT3DDEVICE9 m_pGraphicDev);
	void	CubeInstall(CGameObject* pDefaultCube, LPDIRECT3DDEVICE9 m_pGraphicDev, vector<_vec3>& vecCubePos, int iCubeIndex);
	HRESULT GridON(LPDIRECT3DDEVICE9 m_pGraphicDev, vector<CGameObject*>& vecGrid);
};