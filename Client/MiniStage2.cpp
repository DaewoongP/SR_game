#include "stdafx.h"
#include "MiniStage2.h"
#include "AbstractFactory.h"
#include "Export_Function.h"
#include "Fade.h"
#include "Cube.h"
#include "DynamicCamera.h"
#include "Toodee.h"
#include "MiniTopdee.h"
#include "CrackCube.h"
#include "StageCamera.h"
#include "LaserTurret.h"
#include "ImguiMgr.h"
#include"ImguiBG.h"
#include "MiniGame1Portal.h"
#include "StageBG.h"
CMiniStage2::CMiniStage2(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}


CMiniStage2::~CMiniStage2()
{
}

HRESULT CMiniStage2::Ready_Scene(void)
{
	StopSound(SOUND_BGM);
	PlayBGM(L"Mini2.wav", 0.35f);
	m_eLoadingID = LOADING_MINI2;
	m_pFade = CFade::Create(m_pGraphicDev, false);
	Engine::FrameResetToZero(L"MiniFin");
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);
	CImguiMgr::GetInstance()->Get_BG()->LoadBG(14, this);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	return S_OK;
}

_int CMiniStage2::Update_Scene(const _float & fTimeDelta)
{
	return __super::Update_Scene(fTimeDelta);
}

void CMiniStage2::LateUpdate_Scene(void)
{
	__super::LateUpdate_Scene();
}

void CMiniStage2::Render_Scene(void)
{
}

HRESULT CMiniStage2::Ready_Layer_Environment(const _tchar* pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	FAILED_CHECK_RETURN(FACTORY<CStage1Camera>::Create(L"Camera", pLayer), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CStage1BG>::Create(L"StageBG", pLayer), E_FAIL);

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CMiniStage2::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	FAILED_CHECK_RETURN(FACTORY<CToodee>::Create(L"Toodee", pLayer, _vec3(58.f, 26.f, 10.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CMiniTopdee>::CreateParent(L"MiniTopdee", pLayer, _vec3(3.f, 4.f, 10.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CCrackCube>::Create(L"CrackCube", pLayer, _vec3(60.f, 20.f, 10.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CCrackCube>::Create(L"CrackCube", pLayer, _vec3(58.f, 20.f, 10.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CCrackCube>::Create(L"CrackCube", pLayer, _vec3(56.f, 20.f, 10.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CCrackCube>::Create(L"CrackCube", pLayer, _vec3(54.f, 20.f, 10.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CLaserTurret>::Create(L"LaserTurret", pLayer, _vec3(3.f, 2.f, 10.f), _int(CLaserTurret::LASER_LEFT)), E_FAIL);

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

HRESULT CMiniStage2::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;


	m_uMapLayer.insert({ pLayerTag, pLayer });
	return S_OK;
}

CMiniStage2 * CMiniStage2::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMiniStage2 *	pInstance = new CMiniStage2(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMiniStage2::Free(void)
{
	__super::Free();
}