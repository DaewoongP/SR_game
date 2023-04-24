#include "stdafx.h"
#include "Stage3.h"
#include "AbstractFactory.h"
#include "StageCamera.h"
#include "StageBG.h"
#include "Cube.h"
#include "Toodee.h"
#include "Topdee.h"
#include "Boss3.h"
#include "ImguiMgr.h"
#include "ImguiStage.h"
#include "ImguiUnit.h"
#include "Fade.h"
CStage3::CStage3(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
{
}


CStage3::~CStage3()
{
}

HRESULT CStage3::Ready_Scene(void)
{
	m_eLoadingID = LOADING_STAGE3;
	m_pFade = CFade::Create(m_pGraphicDev, false);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);
	CImguiMgr::GetInstance()->Get_Stage()->LoadCube(2, this);
	CImguiMgr::GetInstance()->Get_Stage()->LoadGrid(2, this);
	CImguiMgr::GetInstance()->Get_Unit()->LoadMapObject(2, this);
	CImguiMgr::GetInstance()->Get_Unit()->LoadMonster(2, this);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	Engine::StopSound(SOUND_BGM);
	Engine::PlayBGM(L"1.wav", 0.35f);
	
	return S_OK;
} 

_int CStage3::Update_Scene(const _float & fTimeDelta)
{
	return __super::Update_Scene(fTimeDelta);
}

void CStage3::LateUpdate_Scene(void)
{
	__super::LateUpdate_Scene();
}

void CStage3::Render_Scene(void)
{
}

HRESULT CStage3::Ready_Layer_Environment(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	FAILED_CHECK_RETURN(FACTORY<CStage1Camera>::Create(L"Camera", pLayer), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CStage1BG>::Create(L"StageBG", pLayer), E_FAIL);

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage3::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	FAILED_CHECK_RETURN(FACTORY<CToodee>::Create(L"Toodee", pLayer, _vec3(60.f, 27.f, 10.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CTopdee>::Create(L"Topdee", pLayer, _vec3(32.f, 32.f, 11.f)), E_FAIL);

	

	m_uMapLayer.insert({ pLayerTag, pLayer });
	return S_OK;
}

HRESULT CStage3::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	m_uMapLayer.insert({ pLayerTag, pLayer });
	return S_OK;
}

CStage3 * CStage3::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage3 *	pInstance = new CStage3(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStage3::Free(void)
{
	__super::Free();
}
