#pragma once
#include"..\Engine\Define.h"

class CImguiMgr
{
	DECLARE_SINGLETON(CImguiMgr)

private:
	explicit CImguiMgr();
	virtual ~CImguiMgr();

public:
	HRESULT Ready_Imgui();


	void Release();
};
