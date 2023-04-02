#pragma once
#include"..\Engine\Define.h"

class CImguiMgr
{

private:
	explicit CImguiMgr();
	~CImguiMgr();

public:
	HRESULT Update_Imgui(LPDIRECT3DDEVICE9		m_pGraphicDev);


	void Release();
};
