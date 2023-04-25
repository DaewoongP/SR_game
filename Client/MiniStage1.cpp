#include "stdafx.h"
#include "MiniStage1.h"
#include "AbstractFactory.h"
#include "Export_Function.h"
#include "Fade.h"
#include "Cube.h"
#include "MoveCamera.h"
#include "DynamicCamera.h"
#include "StageCamera.h"
#include "Topdee.h"
#include "MiniGamePig.h"
#include "FoodCube.h"
#include "MiniGameBat.h"
#include "CrackCube.h"
#include "EvilBat.h"
#include "MiniGame1Portal.h"

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
	//m_pFade = CFade::Create(m_pGraphicDev, false);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	return S_OK;
}

_int CMiniStage1::Update_Scene(const _float & fTimeDelta)
{
	if (g_Is2D)
	{
		CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
		NULL_CHECK_RETURN(pStageLayer, E_FAIL);

		m_dwSpawnTimer += fTimeDelta;
		if (m_dwSpawnTimer > 1)
		{
			//박쥐소환

			//근데 박쥐는 지들끼리 충돌이 안돼야함.
			//상자있는 위치는 못넘어가야함.
			FAILED_CHECK_RETURN(FACTORY<CMiniGameBat>::Create(L"MiniGameBat", pStageLayer, _vec3(0,30,10)), E_FAIL);
			m_dwSpawnTimer = 0;
		}
	}

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

	FAILED_CHECK_RETURN(FACTORY<CStage1Camera>::Create(L"Camera", pLayer), E_FAIL);

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CMiniStage1::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;
	FAILED_CHECK_RETURN(FACTORY<CTopdee>::Create(L"Topdee", pLayer, _vec3(14.f, 32.f, 11.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CMiniGamePig>::Create(L"MiniGamePig", pLayer, _vec3(5.f, 14.f, 8.9f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CFoodCube>::Create(L"FoodCube", pLayer, _vec3(50.f, 14.f, 10.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CEvilBat>::Create(L"EvilBat", pLayer, _vec3(0, 30, 10)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CMiniGame1Portal>::Create(L"MiniGame1Portal", pLayer, _vec3(56, 14, 8.9f)), E_FAIL);
	

	for (int i = 0; i < 54 - 6; i += 2)
	{
		if ((i + 8) == 50)
			continue;
		FAILED_CHECK_RETURN(FACTORY<CCrackCube>::Create(L"CrackCube", pLayer, _vec3(8+i, 14, 10)), E_FAIL);
	}
	

	FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3(4, 10, 10), 1), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3(4, 12, 10), 1), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3(4, 14, 10), 1), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3(4, 16, 10), 1), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3(4, 18, 10), 1), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3(6, 10, 10), 1), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3(6, 14, 10), 1), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3(6, 12, 10), 1), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3(6, 16, 10), 1), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3(6, 18, 10), 1), E_FAIL);

	FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3(54, 10, 10), 1), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3(54, 12, 10), 1), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3(54, 14, 10), 1), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3(54, 16, 10), 1), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3(54, 18, 10), 1), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3(56, 10, 10), 1), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3(56, 14, 10), 1), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3(56, 12, 10), 1), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3(56, 16, 10), 1), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3(56, 18, 10), 1), E_FAIL);

	//중복되지 않는 랜덤 위치에 큐브들을 배치해줄것.


	for (int i = 0; i < CUBEY; i++)
	{
		for (int j = 0; j < CUBEX; j++)
		{
			//맨 윗줄
			if (i == 0)
				FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3{ (_float)j * 2,(_float)i * 2,10.f }, 1), E_FAIL);
			//사이 첫줄
			if (i == CUBEY - 1)
				FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3{ (_float)j * 2,(_float)i * 2,10.f }, 1), E_FAIL);
			//사이 마지막줄
			if (j == 0)
				FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3{ (_float)j * 2,(_float)i * 2,10.f }, 1), E_FAIL);
			//맨 아랫줄
			if (j == CUBEX - 1)
				FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3{ (_float)j * 2,(_float)i * 2,10.f }, 1), E_FAIL);
		}
	}

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