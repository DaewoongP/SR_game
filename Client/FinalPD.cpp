#include "stdafx.h"
#include "FinalPD.h"

#include "AbstractFactory.h"
#include "Export_Function.h"

#include "UICamera.h"

#include "Produce1BackCode.h"
#include "ProduceToodoo.h"
#include "BackgroundSpr.h"
#include "ProduceToodooHand.h"
#include "ProduceErroeWindow.h"
#include "PDGoal.h"
#include "PDSadToodoo.h"
#include "PDBackCode.h"
#include "PDErroe.h"
#include "PDSemicolon.h"


CFinalPD::CFinalPD(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CScene(pGraphicDev)
{
}

CFinalPD::~CFinalPD()
{
}

HRESULT CFinalPD::Ready_Scene(void)
{
	m_eLoadingID = LOADING_PRODUCESCENE2;
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	return S_OK;

}

_int CFinalPD::Update_Scene(const _float & fTimeDelta)
{
	return __super::Update_Scene(fTimeDelta);
}

void CFinalPD::LateUpdate_Scene(void)
{
	__super::LateUpdate_Scene();
}

void CFinalPD::Render_Scene(void)
{
}

HRESULT CFinalPD::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	return S_OK;
}

HRESULT CFinalPD::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	return S_OK;
}

HRESULT CFinalPD::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	pGameObject = CUICamera::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UICamera", pGameObject), E_FAIL);

	pGameObject = CBackgroundSpr::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BackGround", pGameObject), E_FAIL);

	pGameObject = CPDGoal::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PDGoal", pGameObject), E_FAIL);

	pGameObject = CPDSadToodoo::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PDSadToodoo", pGameObject), E_FAIL);


	pGameObject = CPDBackCode::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PDBackCode", pGameObject), E_FAIL);
	
	pGameObject = CPDErroe::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PDErroe", pGameObject), E_FAIL);

	pGameObject = CPDSemicolon::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"PDSemicolon", pGameObject), E_FAIL);

	m_uMapLayer.insert({ pLayerTag, pLayer });
	return S_OK;
}

CFinalPD * CFinalPD::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFinalPD *	pInstance = new CFinalPD(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFinalPD::Free(void)
{
	__super::Free();
}
