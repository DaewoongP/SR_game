#include "stdafx.h"
#include "FinalStage3.h"
#include "AbstractFactory.h"
#include "Export_Function.h"
#include "Cube.h"
#include "ImguiMgr.h"
#include "ImguiStage.h"
#include "ImguiUnit.h"
#include "Fade.h"
#include "DynamicCamera.h"
#include "Boss1.h"
#include "Thirddee.h"
#include "StarBox.h"
#include "ShootingCamera.h"

CFinalStage3::CFinalStage3(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}


CFinalStage3::~CFinalStage3()
{
}

HRESULT CFinalStage3::Ready_Scene(void)
{
	m_eLoadingID = LOADING_FINAL3;
	m_pFade = CFade::Create(m_pGraphicDev, false);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

_int CFinalStage3::Update_Scene(const _float & fTimeDelta)
{
	// 3d 기준으로 처리할거임.
	g_Is2D = false;
	return __super::Update_Scene(fTimeDelta);
}

void CFinalStage3::LateUpdate_Scene(void)
{
	__super::LateUpdate_Scene();
}

void CFinalStage3::Render_Scene(void)
{
}

HRESULT CFinalStage3::Ready_Layer_Environment(const _tchar* pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;
	FAILED_CHECK_RETURN(FACTORY<CShootingCamera>::Create(L"Camera", pLayer), E_FAIL);
	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CFinalStage3::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;
	FAILED_CHECK_RETURN(FACTORY<CThirddee>::Create(L"Thirddee", pLayer, _vec3(10.f, 20.f, 11.f),3), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CStarBox>::Create(L"StarBox", pLayer), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CBoss1>::Create(L"Boss1", pLayer, _vec3(280.f, 25.f, 20.f)), E_FAIL);

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CFinalStage3::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;


	m_uMapLayer.insert({ pLayerTag, pLayer });
	return S_OK;
}

CFinalStage3 * CFinalStage3::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFinalStage3 *	pInstance = new CFinalStage3(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFinalStage3::Free(void)
{
	__super::Free();
}