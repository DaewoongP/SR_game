#include "stdafx.h"
#include "Stage1.h"
#include "AbstractFactory.h"
#include "Export_Function.h"

#include "Cube.h"
#include "StageCamera.h"
#include "MoveCube.h"
#include "StageBG.h"
#include "Key.h"
#include "KeyCube.h"
#include "CrackCube.h"
#include "Pig.h"
#include "Bat.h"
#include "PinkCloud.h"
#include "Topdee.h"
#include "Spike.h"
#include "GravityCube.h"
#include "Portal.h"
#include "Fireball.h"
#include "Spike.h"
#include "InstallGrid.h"

CStage1::CStage1(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}


CStage1::~CStage1()
{
}

HRESULT CStage1::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	return S_OK;
}

_int CStage1::Update_Scene(const _float & fTimeDelta)
{
	return __super::Update_Scene(fTimeDelta);
}

void CStage1::LateUpdate_Scene(void)
{
	__super::LateUpdate_Scene();
}

void CStage1::Render_Scene(void)
{
}

HRESULT CStage1::Ready_Layer_Environment(const _tchar* pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	FAILED_CHECK_RETURN(FACTORY<CStage1Camera>::Create(L"Camera", pLayer), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CStage1BG>::Create(L"StageBG", pLayer), E_FAIL);

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage1::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;
	FAILED_CHECK_RETURN(FACTORY<CPig>::Create(L"Pig", pLayer, _vec3(50.f, 7.f, 10.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CBat>::Create(L"Bat", pLayer, _vec3(50.f, 20.f, 8.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CPinkCloud>::Create(L"PinkCloud", pLayer, _vec3(6.f, 30.f, 10.5f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CMoveCube>::Create(L"MoveCube", pLayer, _vec3(6.f, 8.f, 10.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CGravityCube>::Create(L"GravityCube", pLayer, _vec3(8.f, 10.f, 10.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CGravityCube>::Create(L"GravityCube", pLayer, _vec3(10.f, 12.f, 10.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CGravityCube>::Create(L"GravityCube", pLayer, _vec3(18.f, 30.f, 10.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CGravityCube>::Create(L"GravityCube", pLayer, _vec3(16.f, 28.f, 10.f)), E_FAIL);

	FAILED_CHECK_RETURN(FACTORY<CInstallGrid>::Create(L"InstallGrid", pLayer, _vec3(16.f, 20.f, 10.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CInstallGrid>::Create(L"InstallGrid", pLayer, _vec3(18.f, 22.f, 10.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CInstallGrid>::Create(L"InstallGrid", pLayer, _vec3(18.f, 20.f, 10.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CInstallGrid>::Create(L"InstallGrid", pLayer, _vec3(16.f, 22.f, 10.f)), E_FAIL);

	FAILED_CHECK_RETURN(FACTORY<CToodee>::Create(L"Toodee", pLayer, _vec3(10.f, 7.f, 10.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CTopdee>::Create(L"Topdee", pLayer, _vec3(16.f, 10.f, 11.f)), E_FAIL);
	
	FAILED_CHECK_RETURN(FACTORY<CFireball>::Create(L"Fireball", pLayer, _vec3(40.f, 10.f, 11.f)), E_FAIL);

	int cubeCnt = 0;
	for (int i = 0; i < CUBEY; i++)
	{
		for (int j = 0; j < CUBEX; j++)
		{
			//맨 윗줄
			if (i == 0)
				FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3{ (float)j * 2,(float)i * 2,10.f }), E_FAIL);
			//사이 첫줄
			if (i == CUBEY - 1)
				FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3{ (float)j * 2,(float)i * 2,10.f }), E_FAIL);
			//사이 마지막줄
			if (j == 0)
				FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3{ (float)j * 2,(float)i * 2,10.f }), E_FAIL);
				cubeCnt++;
			//맨 아랫줄
			if (j == CUBEX - 1)
				FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3{ (float)j * 2,(float)i * 2,10.f }), E_FAIL);
		}
	}

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage1::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;


	m_uMapLayer.insert({ pLayerTag, pLayer });
	return S_OK;
}

CStage1 * CStage1::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage1 *	pInstance = new CStage1(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStage1::Free(void)
{
	__super::Free();
}