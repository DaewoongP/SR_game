#include "stdafx.h"
#include "BackGroundToolScene.h"

#include "AbstractFactory.h"

#include "Export_Function.h"

#include "Cube.h"

#include "StageCamera.h"

#include "ImguiMgr.h"
#include "ImguiStage.h"
#include "ImguiUnit.h"

#include "Theme1_Cloud.h"
#include "Theme1_House.h"
#include "Theme1_Sun.h"
#include "Theme1_Tree.h"
#include "Theme1_Wall.h"
#include "Theme1_Cube.h"
#include "Theme2_Bush_0.h"
#include "Theme2_Bush_1.h"
#include "Theme2_Bush_2.h"
#include "Theme2_Bush_3.h"
#include "Theme2_Bush_4.h"
#include "Theme2_PigStatue_0.h"
#include "Theme2_PigStatue_1.h"
#include "Theme2_BatStatue.h"
#include "Theme2_LongTree.h"



CBackGroundToolScene::CBackGroundToolScene(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CScene(pGraphicDev)
{
}

CBackGroundToolScene::~CBackGroundToolScene()
{
}

HRESULT CBackGroundToolScene::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	return S_OK;
}

_int CBackGroundToolScene::Update_Scene(const _float & fTimeDelta)
{
	return __super::Update_Scene(fTimeDelta);
}

void CBackGroundToolScene::LateUpdate_Scene(void)
{
	__super::LateUpdate_Scene();
}

void CBackGroundToolScene::Render_Scene(void)
{
}

HRESULT CBackGroundToolScene::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	FAILED_CHECK_RETURN(FACTORY<CStage1Camera>::Create(L"Camera", pLayer), E_FAIL);

	//FAILED_CHECK_RETURN(FACTORY<CTheme1_Cloud>::Create(L"T1Cloud", pLayer, _vec3(31.f, 17.f, 10.f), 10.f), E_FAIL);
	//FAILED_CHECK_RETURN(FACTORY<CTheme1_House>::Create(L"T1House", pLayer, _vec3(50.f, 20.f, 10.f), 20.f), E_FAIL);
	//FAILED_CHECK_RETURN(FACTORY<CTheme1_Sun>::Create(L"T1Sun", pLayer, _vec3(31.f, 17.f, 10.f), 10.f), E_FAIL);
	//FAILED_CHECK_RETURN(FACTORY<CTheme1_Tree>::Create(L"T1Tree", pLayer, _vec3(11.f, 27.f, 10.f), 10.f), E_FAIL);
	//FAILED_CHECK_RETURN(FACTORY<CTheme1_Wall>::Create(L"T1Wall", pLayer, _vec3(31.f, 07.f, 10.f), 10.f), E_FAIL);
	//FAILED_CHECK_RETURN(FACTORY<CTheme1_Cube>::Create(L"T1Cube", pLayer, _vec3(10.f, 10.f, 10.f), 10.f), E_FAIL);

	FAILED_CHECK_RETURN(FACTORY<CTheme2_Bush_0>::Create(L"T2Bush_0", pLayer, _vec3(31.f, 17.f, 10.f), 10.f), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CTheme2_Bush_1>::Create(L"T2Bush_1", pLayer, _vec3(31.f, 17.f, 10.f), 10.f), E_FAIL);
	//FAILED_CHECK_RETURN(FACTORY<CTheme2_Bush_2>::Create(L"T2Bush_2", pLayer, _vec3(31.f, 17.f, 10.f), 10.f), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CTheme2_Bush_3>::Create(L"T2Bush_3", pLayer, _vec3(31.f, 17.f, 10.f), 10.f), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CTheme2_Bush_4>::Create(L"T2Bush_4", pLayer, _vec3(31.f, 17.f, 10.f), 10.f), E_FAIL);

	FAILED_CHECK_RETURN(FACTORY<CTheme2_PigStatue_0>::Create(L"T2PigStatue_0", pLayer, _vec3(31.f, 17.f, 10.f), 10.f), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CTheme2_PigStatue_1>::Create(L"T2PigStatue_1", pLayer, _vec3(31.f, 17.f, 10.f), 10.f), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CTheme2_BatStatue>::Create(L"T2BatStatue", pLayer, _vec3(31.f, 17.f, 10.f), 10.f), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CTheme2_LongTree>::Create(L"T2LongTree", pLayer, _vec3(31.f, 17.f, 10.f), 10.f), E_FAIL);


	for (int i = 0; i < CUBEY; i++)
	{
		for (int j = 0; j < CUBEX; j++)
		{
			//맨 윗줄
			if (i == 0)
				FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3{ (_float)j * 2,(_float)i * 2,10.f }), E_FAIL);
			//사이 첫줄
			if (i == CUBEY - 1)
				FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3{ (_float)j * 2,(_float)i * 2,10.f }), E_FAIL);
			//사이 마지막줄
			if (j == 0)
				FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3{ (_float)j * 2,(_float)i * 2,10.f }), E_FAIL);
			//맨 아랫줄
			if (j == CUBEX - 1)
				FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3{ (_float)j * 2,(_float)i * 2,10.f }), E_FAIL);
		}
	}

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

CBackGroundToolScene * CBackGroundToolScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBackGroundToolScene *	pInstance = new CBackGroundToolScene(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBackGroundToolScene::Free(void)
{
	__super::Free();
}