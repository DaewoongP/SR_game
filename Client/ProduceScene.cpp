#include "stdafx.h"
#include "ProduceScene.h"

#include "AbstractFactory.h"
#include "Export_Function.h"

#include "UICamera.h"

#include "Produce1BackCode.h"
#include "ProduceToodoo.h"
#include "BackgroundSpr.h"
#include "ProduceToodooHand.h"
#include "ProduceErroeWindow.h"


CProduceScene::CProduceScene(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CScene(pGraphicDev)
{
}

CProduceScene::~CProduceScene()
{
}

HRESULT CProduceScene::Ready_Scene(void)
{
	m_eLoadingID = LOADING_PRODUCESCENE1;
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	StopSound(SOUND_BGM);
	PlayBGM(L"52.wav", 0.1f);
	return S_OK;

}

_int CProduceScene::Update_Scene(const _float & fTimeDelta)
{
	return __super::Update_Scene(fTimeDelta);
}

void CProduceScene::LateUpdate_Scene(void)
{
	__super::LateUpdate_Scene();
}

void CProduceScene::Render_Scene(void)
{
}

HRESULT CProduceScene::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	return S_OK;
}

HRESULT CProduceScene::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	return S_OK;
}

HRESULT CProduceScene::Ready_Layer_UI(const _tchar * pLayerTag)
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

	pGameObject = CProduce1BackCode::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Produce1BackCode", pGameObject), E_FAIL);

	pGameObject = CProduceToodoo::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ProduceToodoo", pGameObject), E_FAIL);

	pGameObject = CProduceToodooHand::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ProduceToodooHand", pGameObject), E_FAIL);

	pGameObject = CProduceErroeWindow::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"ProduceErroeWindow", pGameObject), E_FAIL);

	m_uMapLayer.insert({ pLayerTag, pLayer });
	return S_OK;
}

CProduceScene * CProduceScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CProduceScene *	pInstance = new CProduceScene(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CProduceScene::Free(void)
{
	__super::Free();
}
