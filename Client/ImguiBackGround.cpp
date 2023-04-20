#include "stdafx.h"
#include "ImguiBackGround.h"
#include "ImguiMgr.h"
#include"imgui.h"
#include"imgui_impl_dx9.h"
#include"imgui_impl_win32.h"

CImguiBackGround::CImguiBackGround(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	m_pGraphicDev(pGraphicDev)
{

}

CImguiBackGround::~CImguiBackGround()
{
}

HRESULT CImguiBackGround::Ready_Imgui()
{


	return S_OK;
}

_int CImguiBackGround::Update_Imgui_BackGround()
{
	return _int();
}

void CImguiBackGround::Release()
{
}

void CImguiBackGround::Stage_ComboBox()
{
	const char* szStage[4] = { "Stage_1", "Stage_2", "Stage_3" , "Stage_4" };

	ImGui::Combo("Select Stage", &m_iStageNumber, szStage, 4);
}

void CImguiBackGround::Show_Img()
{
	switch (m_iStageNumber)
	{
	case 0:

		break;

	case 1:

		break;

	case 2:

		break;

	case 3:

		break;
	default:
		break;
	}
}

CImguiBackGround * CImguiBackGround::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}
