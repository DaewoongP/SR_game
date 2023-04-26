#include "stdafx.h"
#include "FinalStage2.h"
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

CFinalStage2::CFinalStage2(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}


CFinalStage2::~CFinalStage2()
{
}

HRESULT CFinalStage2::Ready_Scene(void)
{
	m_eLoadingID = LOADING_FINAL2;
	m_pFade = CFade::Create(m_pGraphicDev, false);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

_int CFinalStage2::Update_Scene(const _float & fTimeDelta)
{
	return __super::Update_Scene(fTimeDelta);
}

void CFinalStage2::LateUpdate_Scene(void)
{
	__super::LateUpdate_Scene();
}

void CFinalStage2::Render_Scene(void)
{
}

HRESULT CFinalStage2::Ready_Layer_Environment(const _tchar* pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;
	FAILED_CHECK_RETURN(FACTORY<CDynamicCamera>::Create(L"Camera", pLayer), E_FAIL);
	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CFinalStage2::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;
	FAILED_CHECK_RETURN(FACTORY<CThirddee>::Create(L"Thirddee", pLayer, _vec3(20.f, 15.f, 11.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CBoss1>::Create(L"Boss1", pLayer, _vec3(80.f, 25.f, 20.f)), E_FAIL);


	// 대충 이정도 사이즈만 하겠다는 뜻.
	for (int i = 0; i < CUBEY; i++)
	{
		for (int j = 0; j < CUBEY; j++)
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
			if (j == CUBEY - 1)
				FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3{ (_float)j * 2,(_float)i * 2,10.f }), E_FAIL);
		}
	}

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CFinalStage2::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;


	m_uMapLayer.insert({ pLayerTag, pLayer });
	return S_OK;
}

CFinalStage2 * CFinalStage2::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFinalStage2 *	pInstance = new CFinalStage2(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFinalStage2::Free(void)
{
	__super::Free();
}