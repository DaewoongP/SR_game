#include "stdafx.h"
#include "Stage.h"
#include "AbstractFactory.h"
#include "Export_Function.h"
#include "Monster.h"
#include "SkyBox.h"
#include "Cube.h"
#include "Grid.h"
#include "DynamiCamera.h"
#include "MoveBox.h"
#include "Topdee.h"
#include "Pig.h"
#include "Bat.h"
#include "CrackBlock.h"
#include "Key.h"
#include "KeyBox.h"
#include "StageBG.h"
#include "Portal.h"
#include "PinkCloud.h"
#include "Spike.h"
#include "InstallGrid.h"
#include "GravityCube.h"
#include"Spike.h"
#include"Fireball.h"


CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}


CStage::~CStage()
{
}

HRESULT CStage::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

_int CStage::Update_Scene(const _float & fTimeDelta)
{
	return __super::Update_Scene(fTimeDelta);
}

void CStage::LateUpdate_Scene(void)
{
	__super::LateUpdate_Scene();
}

void CStage::Render_Scene(void)
{

}


HRESULT CStage::Ready_Layer_Environment(const _tchar* pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	FAILED_CHECK_RETURN(FACTORY<CDynamiCamera>::Create(L"Camera", pLayer), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CStageBG>::Create(L"StageBG", pLayer), E_FAIL);

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;
	FAILED_CHECK_RETURN(FACTORY<CPig>::Create(L"Pig_0", pLayer, _vec3(50.f, 7.f, 10.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CBat>::Create(L"Bat_0", pLayer, _vec3(50.f, 20.f, 8.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CPinkCloud>::Create(L"Cloud_0", pLayer, _vec3(6.f, 30.f, 10.5f)), E_FAIL);

	FAILED_CHECK_RETURN(FACTORY<CToodee>::Create(L"Toodee", pLayer, _vec3(10.f, 7.f, 10.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CTopdee>::Create(L"Topdee", pLayer, _vec3(16.f, 10.f, 11.f)), E_FAIL);

	int cubeCnt = 0;
	for (int i = 0; i < CUBEY; i++)
	{
		for (int j = 0; j < CUBEX; j++)
		{
			//맨 윗줄
			if (i == 0)
			{
				TCHAR objName[128] = { 0 };
				_stprintf_s(objName, _T("MapCube_%d"), (cubeCnt));
				FAILED_CHECK_RETURN(FACTORY<CCube>::Create(objName, pLayer, _vec3{ (float)j * 2,(float)i * 2,10.f }), E_FAIL);
			}

			//사이 첫줄
			if (i == CUBEY - 1)
			{
				TCHAR objName[128] = { 0 };
				_stprintf_s(objName, _T("MapCube_%d"), (cubeCnt));
				FAILED_CHECK_RETURN(FACTORY<CCube>::Create(objName, pLayer, _vec3{ (float)j * 2,(float)i * 2,10.f }), E_FAIL);
			}

			//사이 마지막줄
			if (j == 0)
			{
				TCHAR objName[128] = { 0 };
				_stprintf_s(objName, _T("MapCube_%d"), (cubeCnt));
				FAILED_CHECK_RETURN(FACTORY<CCube>::Create(objName, pLayer, _vec3{ (float)j * 2,(float)i * 2,10.f }), E_FAIL);
				cubeCnt++;
			}

			//맨 아랫줄
			if (j == CUBEX - 1)
			{
				TCHAR objName[128] = { 0 };
				_stprintf_s(objName, _T("MapCube_%d"), (cubeCnt));
				FAILED_CHECK_RETURN(FACTORY<CCube>::Create(objName, pLayer, _vec3{ (float)j * 2,(float)i * 2,10.f }), E_FAIL);
				cubeCnt++;
			}
		}
	}

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;


	m_uMapLayer.insert({ pLayerTag, pLayer });
	return S_OK;
}

CStage * CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage *	pInstance = new CStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStage::Free(void)
{
	__super::Free();
}