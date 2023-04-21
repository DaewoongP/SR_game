#include "stdafx.h"
#include "ImguiBG.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

#include"DefaultBG.h"
#include"Theme1_Cloud.h"
#include"Theme1_Cube.h"
#include"Theme1_House.h"
#include"Theme1_Sun.h"
#include"Theme1_Tree.h"
#include"Theme1_Wall.h"
#include"MapDeco.h"

static _vec3 m_vPos;
static _vec3 vPos;
static _float fX=0;

CImguiBG::CImguiBG(LPDIRECT3DDEVICE9 pGraphicDev):m_pGraphicDev(pGraphicDev)
{
}

CImguiBG::~CImguiBG()
{
}

_int CImguiBG::Update_Imgui_Unit()
{
	BGMenu(); 
	
	if (ImGui::Button("Undo"))
		if (Undo(m_iStageNumber) != S_OK)
			MSG_BOX("Undo Failed");
	

	m_vecGameObject;
	return S_OK;
}


void CImguiBG::Release()
{
}

HRESULT CImguiBG::BGMenu()
{
	if (ImGui::TreeNode("BackGround"))
	{	
		if (ImGui::TreeNode("Stage1"))
		{
			ImGui::Text("Create:F6");
			ImGui::Checkbox("BackGround Install", &m_BG_On);

			const char* items[] = { "T1Cloud", "MapDeco","T1Cube","T1House","T1Sun","T1Tree","T1Wall" };
			ImGui::Combo("BG Type", &m_iBG_Type, items, IM_ARRAYSIZE(items));

			if (1 == m_iBG_Type)
			{
				if (ImGui::Button("LEFT"))
					m_tDecoDir = CD_LEFT;

				ImGui::SameLine();
				if (ImGui::Button("RIGHT"))
					m_tDecoDir = CD_RIGHT;

				ImGui::SameLine();
				if (ImGui::Button("UP"))
					m_tDecoDir = CD_UP;

				ImGui::SameLine();
				if (ImGui::Button("DOWN"))
					m_tDecoDir = CD_DOWN;
			}


			if (m_BG_On && nullptr == m_pDefaultBG)

				CreateDefaultBG();



			if (m_BG_On && nullptr != m_pDefaultBG)
			{
				InstallBG();
				//Preview();

			}
			if (!m_BG_On && nullptr != m_pDefaultBG)
			{
				m_pDefaultBG->m_bDead = true;
				m_pDefaultBG = nullptr;
			}

			Scale();
			ImGui::TreePop();
		}
		//if (ImGui::TreeNode("Stage2"))
		//{
		//	ImGui::Text("Create:F6");
		//	ImGui::Checkbox("BackGround Install", &m_BG_On);

		//	const char* items[] = { "T1Cloud", "MapDeco","T1Cube","T1House","T1Sun","T1Tree","T1Wall" };
		//	ImGui::Combo("BG Type", &m_iBG_Type, items, IM_ARRAYSIZE(items));

		//	if (1 == m_iBG_Type)
		//	{
		//		if (ImGui::Button("LEFT"))
		//			m_tDecoDir = CD_LEFT;

		//		ImGui::SameLine();
		//		if (ImGui::Button("RIGHT"))
		//			m_tDecoDir = CD_RIGHT;

		//		ImGui::SameLine();
		//		if (ImGui::Button("UP"))
		//			m_tDecoDir = CD_UP;

		//		ImGui::SameLine();
		//		if (ImGui::Button("DOWN"))
		//			m_tDecoDir = CD_DOWN;
		//	}


		//	if (m_BG_On && nullptr == m_pDefaultBG)

		//		CreateDefaultBG();



		//	if (m_BG_On && nullptr != m_pDefaultBG)
		//	{
		//		//InstallBG();
		//		//Preview();

		//	}
		//	if (!m_BG_On && nullptr != m_pDefaultBG)
		//	{
		//		m_pDefaultBG->m_bDead = true;
		//		m_pDefaultBG = nullptr;
		//	}

		//	Scale();
		//	ImGui::TreePop();
		//}
		// 저장 기능
		if (ImGui::Button("BackGround Save"))
			FAILED_CHECK_RETURN(SaveBG(m_iStageNumber), E_FAIL);

		// 로드 기능
		ImGui::SameLine();
		if (ImGui::Button("BackGround Load"))
			FAILED_CHECK_RETURN(LoadBG(m_iStageNumber), E_FAIL);
		
	


		ImGui::TreePop();
	}



	return S_OK;
}
void CImguiBG::Preview()
{
	//ImGuiIO& io = ImGui::GetIO();
	//ImTextureID my_tex_id = io.Fonts->TexID;
	////ImTextureID MyTex = 
	//float my_tex_w = (float)io.Fonts->TexWidth;
	//float my_tex_h = (float)io.Fonts->TexHeight;

	//ImVec2 uv_min = ImVec2(0.0f, 0.0f);
	//ImVec2 uv_max = ImVec2(1.0f, 1.0f);
	//ImVec4 tint_col = ImGui::GetStyleColorVec4(ImGuiCol_Text);// : ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
	//ImVec4 border_col = ImGui::GetStyleColorVec4(ImGuiCol_Border);
	//ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);

}
void CImguiBG::Scale()
{
	ImGui::PushItemWidth(300);
	

	ImGui::DragFloat("Scale", &fX); 
	if (fX < 0)
		fX = 0;
	ImGui::PushItemWidth(100);
	
	ImGui::DragFloat("X", &vPos.x);
	ImGui::SameLine();
	ImGui::DragFloat("Y", &vPos.y);
	ImGui::SameLine();
	ImGui::DragFloat("Z", &vPos.z);
	

	//m_pDefaultBG->m_pTransform->m_vInfo[INFO_POS] + _vec3(m_fPos[0], m_fPos[1], m_fPos[2])
}

bool LoadTextureFromFile(const char* filename, LPDIRECT3DTEXTURE9* Out_Texture, int* out_width, int* out_height)
{
	// Load texture from disk
	//PDIRECT3DTEXTURE9 texture;
	D3DSURFACE_DESC my_image_desc;

	//texture->GetLevelDesc(0, &my_image_desc);

	// Retrieve description of the texture surface so we can access its size
	//*Out_Texture = texture;
	//*out_width = (int)my_image_desc.Width;
	//*out_height = (int)my_image_desc.Height;

	

	return true;
 
 
}
void CImguiBG::CreateDefaultBG()
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Environment"));
	NULL_CHECK_RETURN(pStageLayer, );

	FAILED_CHECK_RETURN(FACTORY<CDefaultBG>::Create(L"DefaultBG", pStageLayer, _vec3{ 0.f, 0.f, 0.f }), );

	m_pDefaultBG = Engine::Get_GameObject(L"Layer_Environment", L"DefaultBG");
	
	return;

}

void CImguiBG::CreateBackData()
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Environment"));
	NULL_CHECK_RETURN(pStageLayer, );

}

void CImguiBG::InstallBG()
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Environment"));
	NULL_CHECK_RETURN(pStageLayer, );

	if (Engine::Get_DIKeyState(DIK_F6) == Engine::KEYDOWN)
	{
		OBJINFO tBGInfo = {};
		Stage1Object(pStageLayer);
		
		//tBGInfo.vObjPos = m_pDefaultBG->m_pTransform->m_vInfo[INFO_POS];
		tBGInfo.iObjTypeNumber = m_iBG_Type;
		tBGInfo.pObjtag = m_vecGameObject.back()->m_pTag;
		m_vecBGInfo.push_back(tBGInfo);
	}
	if (!m_vecBGInfo.empty())
	{
		CGameObject* DynamicPos = m_vecGameObject.back();
		//m_vPos = m_vecBGInfo.back().vObjPos;
		if (DynamicPos != nullptr)
			DynamicPos->m_pTransform->m_vInfo[INFO_POS] = vPos + m_vPos;


		int i = 0;
	}
}
void CImguiBG::Stage1Object(CLayer* pStageLayer)
{

	if (0 == m_iBG_Type)
		MakeBG_PS<CTheme1_Cloud>(pStageLayer, L"T1Cloud");

	else if (1 == m_iBG_Type)
		MakeBGNum<CMapDeco>(pStageLayer, L"MapDeco", (_int)m_tDecoDir);

	else if (2 == m_iBG_Type)
		MakeBG_PS<CTheme1_Cube>(pStageLayer, L"T1Cube");

	else if (3 == m_iBG_Type)
		MakeBG_PS<CTheme1_House>(pStageLayer, L"T1House");

	else if (4 == m_iBG_Type)
		MakeBG_PS<CTheme1_Sun>(pStageLayer, L"T1Sun");

	else if (5 == m_iBG_Type)
		MakeBG_PS<CTheme1_Tree>(pStageLayer, L"T1Tree");

	else if (6 == m_iBG_Type)
		MakeBG_PS<CTheme1_Wall>(pStageLayer, L"T1Wall");
}
void CImguiBG::Stage2Object(CLayer* pStageLayer)
{


}
HRESULT CImguiBG::SaveBG(_int iStageNumber)
{
	TCHAR dataFile[128] = { 0 };
	_stprintf_s(dataFile, _T("../Data/BackGround%d.dat"), (iStageNumber + 1));

	HANDLE hFile = CreateFile(dataFile, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD    dwByte = 0;

	for (auto& iter : m_vecBGInfo)
		WriteFile(hFile, &iter, sizeof(OBJINFO), &dwByte, nullptr);

	CloseHandle(hFile);
	return S_OK;
}

HRESULT CImguiBG::LoadBG(_int iStageNumber, CScene* pScene)
{
	m_vecBGInfo.clear();

	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Environment"));
	if (pStageLayer == nullptr)
		pStageLayer = pScene->Get_Layer(L"Layer_Environment");

	TCHAR dataFile[128] = { 0 };
	_stprintf_s(dataFile, _T("../Data/BackGround%d.dat"), (iStageNumber + 1));

	HANDLE hFile = CreateFile(dataFile, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD    dwByte = 0;
	OBJINFO vMapObjectInfo = {};

	while (true)
	{
		ReadFile(hFile, &vMapObjectInfo, sizeof(OBJINFO), &dwByte, nullptr);
		if (dwByte == 0)
			break;
		m_vecBGInfo.push_back(vMapObjectInfo);
	}
	CloseHandle(hFile);
	for (auto& iter : m_vecBGInfo)
	{
		if (0 == iter.iObjTypeNumber)
		{
			FAILED_CHECK_RETURN(FACTORY<CTheme1_Cloud>::Create(L"T1Cloud", pStageLayer, iter.vObjPos,fX), E_FAIL);
		}

		else if (1 == iter.iObjTypeNumber) 
		{
			FAILED_CHECK_RETURN(FACTORY<CMapDeco>::Create(L"MapDeco", pStageLayer, iter.vObjPos,m_tDecoDir), E_FAIL);
		}

		else if (2 == iter.iObjTypeNumber) 
		{
			FAILED_CHECK_RETURN(FACTORY<CTheme1_Cube>::Create(L"T1Cube", pStageLayer, iter.vObjPos,fX), E_FAIL);
		}

		else if (3 == iter.iObjTypeNumber) 
		{
			FAILED_CHECK_RETURN(FACTORY<CTheme1_House>::Create(L"T1House", pStageLayer, iter.vObjPos,fX), E_FAIL);
		}

		else if (4 == iter.iObjTypeNumber) 
		{
			FAILED_CHECK_RETURN(FACTORY<CTheme1_Sun>::Create(L"T1Sun", pStageLayer, iter.vObjPos,fX), E_FAIL);
		}

		else if (5 == iter.iObjTypeNumber) 
		{
			FAILED_CHECK_RETURN(FACTORY<CTheme1_Tree>::Create(L"T1Tree", pStageLayer, iter.vObjPos,fX), E_FAIL);
		}

		else if (6 == iter.iObjTypeNumber)
		{
			FAILED_CHECK_RETURN(FACTORY<CTheme1_Wall>::Create(L"T1Wall", pStageLayer, iter.vObjPos,fX), E_FAIL);
		}

	}

	return S_OK;
}

HRESULT CImguiBG::Undo(_int iStageNumber)
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Environment"));
	NULL_CHECK_RETURN(pStageLayer, E_FAIL);
	if (m_vecGameObject.empty())
		return E_FAIL;
	CGameObject* pGameObject = m_vecGameObject.back();

	if (dynamic_cast<CBackGroundBase*>(pGameObject))
	{
		if (m_vecBGInfo.empty())
			return E_FAIL;
		m_vecBGInfo.pop_back();
	}
	
	pStageLayer->Delete_LastObject(m_vecGameObject.back());
	m_vecGameObject.pop_back();
	return S_OK;
}

CImguiBG* CImguiBG::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CImguiBG* pInstance = new CImguiBG(pGraphicDev);

	if (nullptr == pInstance)
		return nullptr;

	return pInstance;
}


template<typename T>
void CImguiBG::MakeBG(CLayer* pLayer, const _tchar* pObjTag)
{
	CGameObject* pGameObject = nullptr;
	pGameObject = T::Create(m_pGraphicDev,
		m_pDefaultBG->m_pTransform->m_vInfo[INFO_POS]);
	if (pGameObject == nullptr)
		return;
	pGameObject->Sort_Component();
	pLayer->Add_GameObject(pObjTag, pGameObject);
	m_vecGameObject.push_back(pGameObject);
}
template<typename T>
void CImguiBG::MakeBG_PS(CLayer* pLayer, const _tchar* pObjTag)
{
	CGameObject* pGameObject = nullptr;
	pGameObject = T::Create(m_pGraphicDev,
		m_pDefaultBG->m_pTransform->m_vInfo[INFO_POS], fX);
	if (pGameObject == nullptr)
		return;
	pGameObject->Sort_Component();
	pLayer->Add_GameObject(pObjTag, pGameObject);
	m_vecGameObject.push_back(pGameObject);
}
template<typename T>
void CImguiBG::MakeBGNum(CLayer* pLayer, const _tchar* pObjTag, _int iNum)
{
	CGameObject* pGameObject = nullptr;
	pGameObject = T::Create(m_pGraphicDev,
		m_pDefaultBG->m_pTransform->m_vInfo[INFO_POS], iNum);
	if (pGameObject == nullptr)
		return;
	pGameObject->Sort_Component();
	pLayer->Add_GameObject(pObjTag, pGameObject);
	m_vecGameObject.push_back(pGameObject);
}