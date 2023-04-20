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

	FAILED_CHECK_RETURN(FACTORY<CTheme1_Cloud>::Create(L"T1Cloud", pLayer, _vec3(31.f,17.f,10.f),10.f), E_FAIL);


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
