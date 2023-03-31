#include "stdafx.h"
#include "ImguiMgr.h"
#include"imgui.h"
#include"imgui_impl_dx9.h"
#include"imgui_impl_win32.h"
IMPLEMENT_SINGLETON(CImguiMgr)

CImguiMgr::CImguiMgr()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

CImguiMgr::~CImguiMgr()
{


	ImGui::DestroyContext();
}
