#include "stdafx.h"
#include "ImguiBG.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

#include"DefaultBG.h"
#include"Theme.h"

static _vec3 m_vPos;
static _vec3 vPos;
static _float fScale=0;
static _int iAngle=0;
static _int iTexnum = 0;
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
		if (m_BG_On == true)
			m_BG_On2 = false;
		if (m_BG_On2 == true)
			m_BG_On = false;

		if (ImGui::TreeNode("Stage1"))
		{
			ImGui::Text("Create:F6");
			ImGui::Checkbox("BackGround Install", &m_BG_On);

			const char* items[] = { "T1Cloud", "MapDeco","T1Cube","T1House","T1Sun","T1Tree","T1Wall","T1Cow","T1Nibble","T1Floor","Gradation_T1" };
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
			}
			if (!m_BG_On && nullptr != m_pDefaultBG)
			{
				m_pDefaultBG->m_bDead = true;
				m_pDefaultBG = nullptr;
			}

			Scale();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Stage2"))
		{
			ImGui::Text("Create:F6");
			ImGui::Checkbox("BackGround Install", &m_BG_On2);

			const char* items[] = { "T2BatStatue", "T2PigStatue0","T2PigStatue1","T2Bush0","T2Bush1","T2Bush2","T2Bush3","T2Bush4","T2LongTree","T2BigLeaf","Gradation_T2" };
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

			if (m_BG_On2 && nullptr == m_pDefaultBG)

				CreateDefaultBG();


			if (m_BG_On2 && nullptr != m_pDefaultBG)
			{
				InstallBG();
				//Preview();

			}
			if (!m_BG_On2 && nullptr != m_pDefaultBG)
			{
				m_pDefaultBG->m_bDead = true;
				m_pDefaultBG = nullptr;
			}

			Scale();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Stage3"))
		{
			ImGui::Text("Create:F6");
			ImGui::Checkbox("BackGround Install", &m_BG_On3);

			const char* items[] = { "T3AlphaPlate", "T3BrokenPlate","T3Cloud","T3Moss","T3Pattern_0","T3Pattern_1","T3Plate","T3SemicolonPlate","Gradation_T3" };
			ImGui::Combo("BG Type", &m_iBG_Type, items, IM_ARRAYSIZE(items));


			if (m_BG_On3 && nullptr == m_pDefaultBG)
				CreateDefaultBG();

			if (m_BG_On3 && nullptr != m_pDefaultBG)
			{
				InstallBG();
			}
			if (!m_BG_On3 && nullptr != m_pDefaultBG)
			{
				m_pDefaultBG->m_bDead = true;
				m_pDefaultBG = nullptr;
			}

			Scale();
			ImGui::TreePop();
		}
		if (ImGui::TreeNode("Stage4"))
		{
			ImGui::Text("Create:F6");
			ImGui::Checkbox("BackGround Install", &m_BG_On4);

			const char* items[] = { "T4Chimney", "T4Gear8","T4Gear16","T4Smoke_0","T4Smoke_1","Gradation_T4" };
			ImGui::Combo("BG Type", &m_iBG_Type, items, IM_ARRAYSIZE(items));


			if (m_BG_On4 && nullptr == m_pDefaultBG)
				CreateDefaultBG();
			
			if (m_BG_On4 && nullptr != m_pDefaultBG)
			{
				InstallBG();
			}
			if (!m_BG_On4 && nullptr != m_pDefaultBG)
			{
				m_pDefaultBG->m_bDead = true;
				m_pDefaultBG = nullptr;
			}

			Scale();
			ImGui::TreePop();
		}
		if (ImGui::Button("BackGround Save"))
			FAILED_CHECK_RETURN(SaveBG(m_iStageNumber), E_FAIL);

		// ·Îµå ±â´É
		ImGui::SameLine();
		if (ImGui::Button("BackGround Load"))
			LoadBG(m_iStageNumber, Engine::Get_Scene());
	
		ImGui::TreePop();
	}



	return S_OK;
}
void CImguiBG::Preview()
{

}
void CImguiBG::Scale()
{
	ImGui::PushItemWidth(300);
	

	ImGui::DragFloat("Scale", &fScale); 
	ImGui::DragInt("Angle", &iAngle);
	if (fScale <= 0)
		fScale = 1;
	ImGui::PushItemWidth(100);
	
	ImGui::DragFloat("X", &vPos.x);
	ImGui::SameLine();
	ImGui::DragFloat("Y", &vPos.y);
	ImGui::SameLine();
	ImGui::DragFloat("Z", &vPos.z, 0.01f,-2.0f,2.0f);
	if (m_iBG_Type == 9)
		ImGui::InputInt("Texture Number", &iTexnum);
	
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
	if (nullptr == pStageLayer)
		return;

	if (!m_vecGameObject.empty() && (dynamic_cast<CTheme1_Gradation*>(m_vecGameObject.back()) ||
		dynamic_cast<CTheme2_Gradation*>(m_vecGameObject.back()) ||
		dynamic_cast<CTheme3_Gradation*>(m_vecGameObject.back()) ||
		dynamic_cast<CTheme4_Gradation*>(m_vecGameObject.back())))
	{
		fScale = 50.f;
		m_vecBGInfo.back().vObjPos = _vec3(31.f, 17.0f, 12.1f);
		vPos = { 0.0f,0.0f,0.0f };
	}

	if (Engine::Get_DIKeyState(DIK_F6) == Engine::KEYDOWN)
	{
		if (!m_vecBGInfo.empty())
		{
			m_vecBGInfo.back().vObjScale = _vec3(fScale, fScale, 0.0f);
			m_vecBGInfo.back().vObjPos += vPos;
		}
		vPos = { 0.0f,0.0f,-1.0f };
		BGINFO tBGInfo = {};
		
		if(m_BG_On)
			Stage1Object(pStageLayer);
		if(m_BG_On2)
			Stage2Object(pStageLayer);
		if (m_BG_On3)
			Stage3Object(pStageLayer);
		if (m_BG_On4)
			Stage4Object(pStageLayer);

		tBGInfo.vObjScale = _vec3(fScale, fScale, 0.0f);
		tBGInfo.vObjPos = m_vecGameObject.back()->m_pTransform->m_vInfo[INFO_POS];
		tBGInfo.iObjTypeNumber = m_iBG_Type;
		tBGInfo.fAngle = iAngle;
		tBGInfo.iTexnum = iTexnum;
		
		
		m_vecBGInfo.push_back(tBGInfo);
	}
	if (!m_vecGameObject.empty())
	{
		CGameObject* DynamicPos = m_vecGameObject.back();
		m_vPos = m_vecBGInfo.back().vObjPos;
		if (DynamicPos != nullptr)
		{
			m_vecGameObject.back()->m_pTransform->m_vInfo[INFO_POS] = m_vPos + vPos;
			m_vecGameObject.back()->m_pTransform->m_vScale = { fScale,fScale,1.f };
			m_vecGameObject.back()->m_pTransform->m_vAngle.z = iAngle;
		}
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
	else if (7 == m_iBG_Type)
		MakeBG_PS<CTheme1_Cow>(pStageLayer, L"T1Cow");
	else if (8 == m_iBG_Type)
		MakeBG_PS<CTheme1_Nibble>(pStageLayer, L"T1Nibble");
	else if (9 == m_iBG_Type)
		MakeBGTexNum<CTheme1_Floor>(pStageLayer, L"T1Floor",iTexnum);
	else if (10 == m_iBG_Type)
		MakeBG_PS<CTheme1_Gradation>(pStageLayer, L"Theme1_Gradation");
}
void CImguiBG::Stage2Object(CLayer* pStageLayer)
{
	if (0 == m_iBG_Type)
		MakeBG_PS<CTheme2_BatStatue>(pStageLayer, L"T2BatStatue");

	else if (1 == m_iBG_Type)
		MakeBG_PS<CTheme2_PigStatue_0>(pStageLayer, L"T2PigStatue0");

	else if (2 == m_iBG_Type)
		MakeBG_PS<CTheme2_PigStatue_1>(pStageLayer, L"T2PigStatue1");

	else if (3 == m_iBG_Type)
		MakeBG_PS<CTheme2_Bush_0>(pStageLayer, L"T2Bush0");

	else if (4 == m_iBG_Type)
		MakeBG_PS<CTheme2_Bush_1>(pStageLayer, L"T2Bush1");
	
	else if (5 == m_iBG_Type)
		MakeBG_PS<CTheme2_Bush_2>(pStageLayer, L"T2Bush2");

	else if (6 == m_iBG_Type)
		MakeBG_PS<CTheme2_Bush_3>(pStageLayer, L"T2Bush3");

	else if (7 == m_iBG_Type)
		MakeBG_PS<CTheme2_Bush_4>(pStageLayer, L"T2Bush4");
	
	else if (8 == m_iBG_Type)
		MakeBG_PS<CTheme2_LongTree>(pStageLayer, L"T2LongTree");

	else if (9 == m_iBG_Type)
		MakeBG_PS<CTheme2_BigLeaf>(pStageLayer, L"T2BigLeaf");

	else if (10 == m_iBG_Type)
		MakeBG_PS<CTheme2_Gradation>(pStageLayer, L"Theme2_Gradation");
}
void CImguiBG::Stage3Object(CLayer* pStageLayer)
{
	if (0 == m_iBG_Type)
		MakeBG_PS<CTheme3_AlphaPlate>(pStageLayer, L"T3AlphaPlate");

	else if (1 == m_iBG_Type)
		MakeBG_PS<CTheme3_BrokenPlate>(pStageLayer, L"T3BrokenPlate");

	else if (2 == m_iBG_Type)
		MakeBG_PS<CTheme3_Cloud>(pStageLayer, L"T3Cloud");

	else if (3 == m_iBG_Type)
		MakeBG_PS<CTheme3_Moss>(pStageLayer, L"T3Moss");

	else if (4 == m_iBG_Type)
		MakeBG_PS<CTheme3_Pattern_0>(pStageLayer, L"T3Pattern_0");

	else if (5 == m_iBG_Type)
		MakeBG_PS<CTheme3_Pattern_1>(pStageLayer, L"T3Pattern_1");

	else if (6 == m_iBG_Type)
		MakeBG_PS<CTheme3_Plate>(pStageLayer, L"T3Plate");

	else if (7 == m_iBG_Type)
		MakeBG_PS<CTheme3_SemicolonPlate>(pStageLayer, L"T3SemicolonPlate");

	else if (8 == m_iBG_Type)
		MakeBG_PS<CTheme3_Gradation>(pStageLayer, L"Theme3_Gradation");

}
void CImguiBG::Stage4Object(CLayer* pStageLayer)
{
	if (0 == m_iBG_Type)
		MakeBG_PS<CTheme4_Chimney>(pStageLayer, L"T4Chimney");

	else if (1 == m_iBG_Type)
		MakeBG_PS<CTheme4_Gear8>(pStageLayer, L"T4Gear8");

	else if (2 == m_iBG_Type)
		MakeBG_PS<CTheme4_Gear16>(pStageLayer, L"T4Gear16");

	else if (3 == m_iBG_Type)
		MakeBG_PS<CTheme4_Smoke_0>(pStageLayer, L"T4Smoke_0");

	else if (4 == m_iBG_Type)
		MakeBG_PS<CTheme4_Smoke_1>(pStageLayer, L"T4Smoke_1");

	else if (5 == m_iBG_Type)
		MakeBG_PS<CTheme4_Gradation>(pStageLayer, L"Theme4_Gradation");
}


HRESULT CImguiBG::SaveBG(_int iStageNumber)
{
	TCHAR dataFile[128] = { 0 };
	_stprintf_s(dataFile, _T("../Data/BackGround%d.dat"), (iStageNumber + 1));

	HANDLE hFile = CreateFile(dataFile, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD    dwByte = 0;
	if (!m_vecBGInfo.empty())
	{
		m_vecBGInfo.back().vObjScale = _vec3(fScale, fScale, 0.0f);
		m_vecBGInfo.back().vObjPos += vPos;
	}
	for (auto& iter : m_vecBGInfo)
		WriteFile(hFile, &iter, sizeof(BGINFO), &dwByte, nullptr);

	CloseHandle(hFile);
	return S_OK;
}

HRESULT CImguiBG::LoadBG(_int iStageNumber, CScene* pScene)
{
	int i = 0;
	m_vecBGInfo.clear();
	m_vecGameObject.clear();

	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_Environment"));
	if (pStageLayer == nullptr)
		pStageLayer = pScene->Get_Layer(L"Layer_Environment");

	TCHAR dataFile[128] = { 0 };
	_stprintf_s(dataFile, _T("../Data/BackGround%d.dat"), (iStageNumber + 1));

	HANDLE hFile = CreateFile(dataFile, GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (INVALID_HANDLE_VALUE == hFile)
		return 0 ;

	DWORD    dwByte = 0;
	BGINFO vMapObjectInfo = {};

	while (true)
	{
		ReadFile(hFile, &vMapObjectInfo, sizeof(BGINFO), &dwByte, nullptr);
		if (dwByte == 0)
			break;
		m_vecBGInfo.push_back(vMapObjectInfo);
	}
	CloseHandle(hFile);
	if(m_BG_On|| iStageNumber==0 || iStageNumber == 1 )
	for (auto& iter : m_vecBGInfo)
	{
		if (0 == iter.iObjTypeNumber)
		{
			FAILED_CHECK_RETURN(FACTORY<CTheme1_Cloud>::Create(L"T1Cloud", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
		}

		else if (1 == iter.iObjTypeNumber) 
		{
			FAILED_CHECK_RETURN(FACTORY<CMapDeco>::Create(L"MapDeco", pStageLayer, iter.vObjPos,m_tDecoDir), E_FAIL);
		}

		else if (2 == iter.iObjTypeNumber) 
		{
			FAILED_CHECK_RETURN(FACTORY<CTheme1_Cube>::Create(L"T1Cube", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
		}

		else if (3 == iter.iObjTypeNumber) 
		{
			FAILED_CHECK_RETURN(FACTORY<CTheme1_House>::Create(L"T1House", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
		}

		else if (4 == iter.iObjTypeNumber) 
		{
			FAILED_CHECK_RETURN(FACTORY<CTheme1_Sun>::Create(L"T1Sun", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
		}

		else if (5 == iter.iObjTypeNumber) 
		{
			FAILED_CHECK_RETURN(FACTORY<CTheme1_Tree>::Create(L"T1Tree", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
		}

		else if (6 == iter.iObjTypeNumber)
		{
			FAILED_CHECK_RETURN(FACTORY<CTheme1_Wall>::Create(L"T1Wall", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
		}
		else if (7 == iter.iObjTypeNumber)
		{
			FAILED_CHECK_RETURN(FACTORY<CTheme1_Cow>::Create(L"T1Cow", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
		}
		else if (8 == iter.iObjTypeNumber)
		{
			FAILED_CHECK_RETURN(FACTORY<CTheme1_Cow>::Create(L"T1Nibble", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
		}
		else if (9 == iter.iObjTypeNumber)
		{
			FAILED_CHECK_RETURN(FACTORY<CTheme1_Floor>::Create(L"T1Floor", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.iTexnum), E_FAIL);
		}
		else if (10 == iter.iObjTypeNumber)
		{
			FAILED_CHECK_RETURN(FACTORY<CTheme1_Gradation>::Create(L"Theme1_Gradation", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
		}
	}
	if (m_BG_On2 || iStageNumber == 2 || iStageNumber == 3 || iStageNumber == 4 || iStageNumber == 13)
	{
		for (auto& iter : m_vecBGInfo)
		{

			if (0 == iter.iObjTypeNumber)
			{
				FAILED_CHECK_RETURN(FACTORY<CTheme2_BatStatue>::Create(L"T2BatStatue", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
			}

			else if (1 == iter.iObjTypeNumber)
			{
				FAILED_CHECK_RETURN(FACTORY<CTheme2_PigStatue_0>::Create(L"T2PigStatue0", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
			}

			else if (2 == iter.iObjTypeNumber)
			{
				FAILED_CHECK_RETURN(FACTORY<CTheme2_PigStatue_1>::Create(L"T2PigStatue1", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
			}

			else if (3 == iter.iObjTypeNumber)
			{
				FAILED_CHECK_RETURN(FACTORY<CTheme2_Bush_0>::Create(L"T2Bush0", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
			}

			else if (4 == iter.iObjTypeNumber)
			{
				FAILED_CHECK_RETURN(FACTORY<CTheme2_Bush_1>::Create(L"T2Bush1", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
			}

			else if (5 == iter.iObjTypeNumber)
			{
				FAILED_CHECK_RETURN(FACTORY<CTheme2_Bush_2>::Create(L"T2Bush2", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
			}

			else if (6 == iter.iObjTypeNumber)
			{
				FAILED_CHECK_RETURN(FACTORY<CTheme2_Bush_3>::Create(L"T2Bush3", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
			}

			else if (7 == iter.iObjTypeNumber)
			{
				FAILED_CHECK_RETURN(FACTORY<CTheme2_Bush_4>::Create(L"T2Bush4", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
			}

			else if (8 == iter.iObjTypeNumber)
			{
				FAILED_CHECK_RETURN(FACTORY<CTheme2_LongTree>::Create(L"T2LongTree", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
			}
			else if (9 == iter.iObjTypeNumber)
			{
				FAILED_CHECK_RETURN(FACTORY<CTheme2_BigLeaf>::Create(L"T2BigLeaf", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
			}

			else if (10 == iter.iObjTypeNumber)
			{
				FAILED_CHECK_RETURN(FACTORY<CTheme2_Gradation>::Create(L"Theme2_Gradation", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
			}
		}
	}
	if (m_BG_On3 || iStageNumber == 5 || iStageNumber == 6 || iStageNumber == 7 || iStageNumber == 14)
	{
		for (auto& iter : m_vecBGInfo)
		{

			if (0 == iter.iObjTypeNumber)
			{
				FAILED_CHECK_RETURN(FACTORY<CTheme3_AlphaPlate>::Create(L"T3AlphaPlate", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
			}

			else if (1 == iter.iObjTypeNumber)
			{
				FAILED_CHECK_RETURN(FACTORY<CTheme3_BrokenPlate>::Create(L"T3BrokenPlate", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
			}

			else if (2 == iter.iObjTypeNumber)
			{
				FAILED_CHECK_RETURN(FACTORY<CTheme3_Cloud>::Create(L"T3Cloud", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
			}

			else if (3 == iter.iObjTypeNumber)
			{
				FAILED_CHECK_RETURN(FACTORY<CTheme3_Moss>::Create(L"T3Moss", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
			}

			else if (4 == iter.iObjTypeNumber)
			{
				FAILED_CHECK_RETURN(FACTORY<CTheme3_Pattern_0>::Create(L"T3Pattern_0", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
			}

			else if (5 == iter.iObjTypeNumber)
			{
				FAILED_CHECK_RETURN(FACTORY<CTheme3_Pattern_1>::Create(L"T3Pattern_1", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
			}

			else if (6 == iter.iObjTypeNumber)
			{
				FAILED_CHECK_RETURN(FACTORY<CTheme3_Plate>::Create(L"T3Plate", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
			}

			else if (7 == iter.iObjTypeNumber)
			{
				FAILED_CHECK_RETURN(FACTORY<CTheme3_SemicolonPlate>::Create(L"T3SemicolonPlate", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
			}

			else if (8 == iter.iObjTypeNumber)
			{
				FAILED_CHECK_RETURN(FACTORY<CTheme3_Gradation>::Create(L"Theme3_Gradation", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
			}

		}
	}
	if (m_BG_On4 || iStageNumber == 8 || iStageNumber == 9)
	{
		for (auto& iter : m_vecBGInfo)
		{

			if (0 == iter.iObjTypeNumber)
			{
				FAILED_CHECK_RETURN(FACTORY<CTheme4_Chimney>::Create(L"T4Chimney", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
			}

			else if (1 == iter.iObjTypeNumber)
			{
				FAILED_CHECK_RETURN(FACTORY<CTheme4_Gear8>::Create(L"T4Gear8", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
			}

			else if (2 == iter.iObjTypeNumber)
			{
				FAILED_CHECK_RETURN(FACTORY<CTheme4_Gear16>::Create(L"T4Gear16", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
			}

			else if (3 == iter.iObjTypeNumber)
			{
				FAILED_CHECK_RETURN(FACTORY<CTheme4_Smoke_0>::Create(L"T4Smoke_0", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
			}

			else if (4 == iter.iObjTypeNumber)
			{
				FAILED_CHECK_RETURN(FACTORY<CTheme4_Smoke_1>::Create(L"T4Smoke_1", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
			}

			else if (5 == iter.iObjTypeNumber)
			{
				FAILED_CHECK_RETURN(FACTORY<CTheme4_Gradation>::Create(L"Theme4_Gradation", pStageLayer, iter.vObjPos, iter.vObjScale.x, iter.fAngle), E_FAIL);
			}
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
		m_pDefaultBG->m_pTransform->m_vInfo[INFO_POS] + _vec3(0.0f, 0.0f, 1.1f));
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
		m_pDefaultBG->m_pTransform->m_vInfo[INFO_POS] + _vec3(0.0f, 0.0f, 1.1f), fScale,0.0f);
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
		m_pDefaultBG->m_pTransform->m_vInfo[INFO_POS] + _vec3(0.0f, 0.0f, 1.1f), iNum);
	if (pGameObject == nullptr)
		return;
	pGameObject->Sort_Component();
	pLayer->Add_GameObject(pObjTag, pGameObject);
	m_vecGameObject.push_back(pGameObject);
}
template<typename T>
void CImguiBG::MakeBGTexNum(CLayer* pLayer, const _tchar* pObjTag, _int iNum)
{
	CGameObject* pGameObject = nullptr;
	pGameObject = T::Create(m_pGraphicDev,
		m_pDefaultBG->m_pTransform->m_vInfo[INFO_POS] + _vec3(0.0f, 0.0f, 1.05f),fScale, iNum);
	if (pGameObject == nullptr)
		return;
	pGameObject->Sort_Component();
	pLayer->Add_GameObject(pObjTag, pGameObject);
	m_vecGameObject.push_back(pGameObject);
}