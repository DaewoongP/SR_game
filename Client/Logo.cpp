#include "stdafx.h"
#include "Logo.h"

#include "AbstractFactory.h"

#include "Export_Function.h"
#include "BackGround.h"
#include "Title.h"
#include "UICamera.h"
#include "StageCamera.h"
#include "LogoCamera.h"
#include "BackgroundSpr.h"
#include "ShiningStar.h"
#include "MenuSmoke.h"
#include "MenuCubeSpr.h"
#include "Select.h"
#include "PreStage.h"
#include "Stage1.h"

_bool CLogo::m_bStart = true;
CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}


CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	return S_OK;
}

_int CLogo::Update_Scene(const _float & fTimeDelta)
{
	int iExit = __super::Update_Scene(fTimeDelta);
	if (Engine::Get_DIKeyState(DIK_UP) == Engine::KEYDOWN)
	{	
		StopSound(SOUND_CAM);
		PlaySound_Effect(L"61.wav", SOUND_CAM, 1.f); 
		m_bStart = true;
	}
	if (Engine::Get_DIKeyState(DIK_DOWN) == Engine::KEYDOWN)
	{
			StopSound(SOUND_CAM);
			PlaySound_Effect(L"62.wav", SOUND_CAM, 1.f);
		m_bStart = false;
	}
	if (Engine::Get_DIKeyState(DIK_RETURN) == Engine::KEYDOWN && m_bStart == true)
	{
		StopSound(SOUND_CAM);
		PlaySound_Effect(L"51.wav", SOUND_CAM, 1.f);
		CScene*	pScene = CPreStage::Create(m_pGraphicDev, LOADING_FINAL1);
		NULL_CHECK_RETURN(pScene, -1);

		FAILED_CHECK_RETURN(Engine::Set_Scene(pScene), E_FAIL);
		pScene->Update_Scene(fTimeDelta);
		return 0;
	}
	else if (GetAsyncKeyState(VK_RETURN) && m_bStart == false)
		PostQuitMessage(0);

	return iExit;
}

void CLogo::LateUpdate_Scene(void)
{
	__super::LateUpdate_Scene();
}

void CLogo::Render_Scene(void)
{
}

HRESULT CLogo::Ready_Layer_Environment(const _tchar* pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	pGameObject = CBackgroundSpr::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BackGround", pGameObject), E_FAIL);

	for (size_t i = 0; i < 50; i++)
	{
		_float fRandX = GetRandomFloat(-WINCX*0.5f, WINCX*0.5f);
		_float fRandY = GetRandomFloat(-WINCY*0.5f, WINCY*0.5f);

		FAILED_CHECK_RETURN(FACTORY<CShiningStar>::Create(L"ShiningStar", pLayer, _vec3(fRandX, fRandY, 3.f)), E_FAIL);
	}

	FAILED_CHECK_RETURN(FACTORY<CMenuCubeSpr>::Create(L"MenuCube", pLayer, _vec3(300.0f, -300.0f, 2.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CMenuCubeSpr>::Create(L"MenuCube", pLayer, _vec3(-300.0f, 200.0f, 2.f)), E_FAIL);

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}


HRESULT CLogo::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	pGameObject = CUICamera::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UICamera", pGameObject), E_FAIL);

	pGameObject = CTitle::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Title", pGameObject), E_FAIL);

	pGameObject = CSelect::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Select", pGameObject), E_FAIL);


	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

CLogo * CLogo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogo *	pInstance = new CLogo(pGraphicDev);
	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLogo::Free(void)
{
	__super::Free();
}
