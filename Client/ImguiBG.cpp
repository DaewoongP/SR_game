#include "stdafx.h"
#include "ImguiBG.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

#include"DefaultBG.h"
#include"BackCloud.h"
#include"MapDeco.h"


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

	return S_OK;
}


void CImguiBG::Release()
{
}

HRESULT CImguiBG::BGMenu()
{
	if (ImGui::TreeNode("BackGround"))
	{	
		ImGui::Text("Create:F6");
		ImGui::Checkbox("BackGround Install", &m_BG_On);

		const char* items[] = { "BackCloud", "MapDeco" };
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

void CImguiBG::InstallBG()
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Environment"));
	NULL_CHECK_RETURN(pStageLayer, );

	if (Engine::Get_DIKeyState(DIK_F6) == Engine::KEYDOWN)
	{
		OBJINFO tBGInfo = {};

		if (0 == m_iBG_Type) 
			MakeBG<CBackCloud>(pStageLayer, L"BackCloud");

		else if (1 == m_iBG_Type)
			MakeBGNum<CMapDeco>(pStageLayer, L"MapDeco",(_int)m_tDecoDir);

		tBGInfo.vObjPos = m_pDefaultBG->m_pTransform->m_vInfo[INFO_POS];
		tBGInfo.iObjTypeNumber = m_iBG_Type;

		m_vecBGInfo.push_back(tBGInfo);
	}


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
	

	return S_OK;
}

HRESULT CImguiBG::Undo(_int iStageNumber)
{
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Environment"));
	NULL_CHECK_RETURN(pStageLayer, E_FAIL);
	if (m_vecGameObject.empty())
		return E_FAIL;
	CGameObject* pGameObject = m_vecGameObject.back();

	/*if (dynamic_cast<Cback*>(pGameObject))
	{
		if (m_vecMonsterInfo.empty())
			return E_FAIL;
		m_vecMonsterInfo.pop_back();
	}
	else
	{
		if (dynamic_cast<CPortalCube*>(pGameObject))
			--m_iPortalCubeCount;
		if (m_vecMapObjectInfo.empty())
			return E_FAIL;
		m_vecMapObjectInfo.pop_back();
	}*/
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