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
#include "Final3Boss1.h"
#include "StarBox.h"
#include "ShootingCamera.h"
#include "ShootingPlayer.h"
#include"FinalUI.h"
#include"FinalUI_B.h"
#include "Item.h"
#include "StageCamera.h"

#include "Item.h"

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
	//FAILED_CHECK_RETURN(FACTORY<CStage1Camera>::Create(L"Camera", pLayer), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CShootingCamera>::Create(L"ShootingCamera", pLayer), E_FAIL);
	//FAILED_CHECK_RETURN(FACTORY<CDynamicCamera>::Create(L"Camera", pLayer), E_FAIL);	
	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CFinalStage3::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;
	FAILED_CHECK_RETURN(FACTORY<CShootingPlayer>::Create(L"Thirddee", pLayer, _vec3(0.f, 0.f, 15.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CStarBox>::Create(L"StarBox", pLayer), E_FAIL);

	FAILED_CHECK_RETURN(FACTORY<CFinal3Boss1>::Create(L"Final3Boss1", pLayer, _vec3(20.f, 200.f, 30.f)), E_FAIL);

	// 카메라 확인용, 큐브 위치 확인용 나중에 삭제합시다 이건


	FAILED_CHECK_RETURN(FACTORY<CItem>::Create(L"Item", pLayer, _vec3(-6.f, 200.f, 10.f), 0), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CItem>::Create(L"Item", pLayer, _vec3(-3.f, 250.f, 15.f), 1), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CItem>::Create(L"Item", pLayer, _vec3(3.f, 300.f, 15.f), 2), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CItem>::Create(L"Item", pLayer, _vec3(6.f, 350.f, 10.f), 3), E_FAIL);

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CFinalStage3::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);
	_int pHp = Get_GameObject(L"Layer_GameLogic", L"Final3Boss1")->Get_Hp();
	FAILED_CHECK_RETURN(FACTORY<CFinalUI_B>::Create(L"FinalUI", pLayer, pHp), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CFinalUI>::Create(L"FinalUI", pLayer, pHp), E_FAIL);

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