#include "stdafx.h"
#include "Stage9.h"
#include "AbstractFactory.h"
#include "StageCamera.h"
#include "StageBG.h"
#include "Cube.h"
#include "Toodee.h"
#include "Topdee.h"
#include"Tookee.h"
#include "ImguiMgr.h"
#include "ImguiStage.h"
#include "ImguiUnit.h"
#include "Fade.h"
CStage9::CStage9(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
{
}


CStage9::~CStage9()
{
}

HRESULT CStage9::Ready_Scene(void)
{
	m_eLoadingID = LOADING_STAGE9;
	m_pFade = CFade::Create(m_pGraphicDev, false);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);
	CImguiMgr::GetInstance()->Get_Stage()->LoadCube(8, this);
	CImguiMgr::GetInstance()->Get_Stage()->LoadGrid(8, this);
	CImguiMgr::GetInstance()->Get_Unit()->LoadMapObject(8, this);
	CImguiMgr::GetInstance()->Get_Unit()->LoadMonster(8, this);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	Engine::StopSound(SOUND_BGM);
	Engine::PlayBGM(L"0.wav", 0.35f);
	return S_OK;
}

_int CStage9::Update_Scene(const _float& fTimeDelta)
{
	return __super::Update_Scene(fTimeDelta);
}

void CStage9::LateUpdate_Scene(void)
{
	__super::LateUpdate_Scene();
}

void CStage9::Render_Scene(void)
{
}

HRESULT CStage9::Ready_Layer_Environment(const _tchar* pLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject* pGameObject = nullptr;

	FAILED_CHECK_RETURN(FACTORY<CStage1Camera>::Create(L"Camera", pLayer), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CStage1BG>::Create(L"StageBG", pLayer), E_FAIL);

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage9::Ready_Layer_GameLogic(const _tchar* pLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject* pGameObject = nullptr;

	FAILED_CHECK_RETURN(FACTORY<CToodee>::Create(L"Toodee", pLayer, _vec3(60.f, 4.f, 10.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CTopdee>::Create(L"Topdee", pLayer, _vec3(8.f, 4.f, 11.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CTookee>::Create(L"Tookee", pLayer, _vec3(16.f, 4.f, 11.f)), E_FAIL);

	m_uMapLayer.insert({ pLayerTag, pLayer });
	return S_OK;
}

HRESULT CStage9::Ready_Layer_UI(const _tchar* pLayerTag)
{
	CLayer* pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject* pGameObject = nullptr;


	m_uMapLayer.insert({ pLayerTag, pLayer });
	return S_OK;
}

CStage9* CStage9::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage9* pInstance = new CStage9(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStage9::Free(void)
{
	__super::Free();
}
