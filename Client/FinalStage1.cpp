#include "stdafx.h"
#include "FinalStage1.h"
#include "AbstractFactory.h"
#include "Export_Function.h"
#include "Cube.h"
#include "StageCamera.h"
#include "StageBG.h"
#include "ImguiMgr.h"
#include "ImguiStage.h"
#include "ImguiUnit.h"
#include "ThirdCamera.h"
#include "Toodee.h"
#include "Topdee.h"
#include "MoveCamera.h"
#include "Thirddee.h"
#include "Boss1.h"
#include "SkyBox.h"
#include "MoveCube.h"
#include "Fade.h"
#include "FinalPortal.h"
#include "FinalPortalFloor.h"
#include "Semicolon.h"
#include "DynamicCamera.h"
#include "InvisibleGrid.h"

CFinalStage1::CFinalStage1(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}


CFinalStage1::~CFinalStage1()
{
}

HRESULT CFinalStage1::Ready_Scene(void)
{

	m_eLoadingID = LOADING_FINAL1;
	m_pFade = CFade::Create(m_pGraphicDev, false);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	Engine::StopSound(SOUND_BGM);
	Engine::PlayBGM(L"3.wav", 0.35f);
	
	return S_OK;
}

_int CFinalStage1::Update_Scene(const _float & fTimeDelta)
{
	return __super::Update_Scene(fTimeDelta);
}

void CFinalStage1::LateUpdate_Scene(void)
{
	__super::LateUpdate_Scene();
}

void CFinalStage1::Render_Scene(void)
{
}

HRESULT CFinalStage1::Ready_Layer_Environment(const _tchar* pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	FAILED_CHECK_RETURN(FACTORY<CThirdCamera>::Create(L"Camera", pLayer), E_FAIL);
	//FAILED_CHECK_RETURN(FACTORY<CDynamicCamera>::Create(L"Camera", pLayer), E_FAIL);
	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CFinalStage1::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	FAILED_CHECK_RETURN(FACTORY<CThirddee>::Create(L"Thirddee", pLayer, _vec3(34.f, 15.f, 11.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CBoss1>::Create(L"Boss1", pLayer, _vec3(270.f, -50.f, 50.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CMoveCube>::Create(L"MoveCube", pLayer, _vec3(60.f, 23.f, 11.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CFinalPortal>::CreateParent(L"FinalPortal", pLayer, _vec3(180.f, 16.f , 7.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CSemicolon>::Create(L"SemiColon", pLayer, _vec3(180.1f, 16.f, 5.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CSkyBox>::Create(L"SkyBox", pLayer), E_FAIL);

	for (int i = 0; i < CUBEY; i++)
	{
		for (int j = 0; j < CUBEX * 3; j++)
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
			if (j == CUBEX * 3 - 1)
				FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3{ (_float)j * 2,(_float)i * 2,10.f }), E_FAIL);
		}
	}

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CFinalStage1::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;


	m_uMapLayer.insert({ pLayerTag, pLayer });
	return S_OK;
}

CFinalStage1 * CFinalStage1::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFinalStage1 *	pInstance = new CFinalStage1(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFinalStage1::Free(void)
{
	__super::Free();
}