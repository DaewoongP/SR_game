#include "stdafx.h"
#include "MiniStage1.h"
#include "AbstractFactory.h"
#include "Export_Function.h"
#include "Fade.h"
#include "Cube.h"
#include "MoveCamera.h"
#include "DynamicCamera.h"

CMiniStage1::CMiniStage1(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}


CMiniStage1::~CMiniStage1()
{
}

HRESULT CMiniStage1::Ready_Scene(void)
{
	m_eLoadingID = LOADING_MINI1;
	m_pFade = CFade::Create(m_pGraphicDev, false);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	return S_OK;
}

_int CMiniStage1::Update_Scene(const _float & fTimeDelta)
{
	return __super::Update_Scene(fTimeDelta);
}

void CMiniStage1::LateUpdate_Scene(void)
{
	__super::LateUpdate_Scene();
}

void CMiniStage1::Render_Scene(void)
{
}

HRESULT CMiniStage1::Ready_Layer_Environment(const _tchar* pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	FAILED_CHECK_RETURN(FACTORY<CDynamicCamera>::Create(L"Camera", pLayer), E_FAIL);

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CMiniStage1::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CMiniStage1::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;


	m_uMapLayer.insert({ pLayerTag, pLayer });
	return S_OK;
}

CMiniStage1 * CMiniStage1::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMiniStage1 *	pInstance = new CMiniStage1(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMiniStage1::Free(void)
{
	__super::Free();
}