#include "stdafx.h"
#include "Logo.h"

#include "AbstractFactory.h"

#include "Export_Function.h"
#include "BackGround.h"
#include"Title.h"
#include "Stage1.h"
#include "UICamera.h"
#include "StageCamera.h"
#include "LogoCamera.h"
#include "BackgroundSpr.h"
#include "ShiningStar.h"
#include "MenuSmoke.h"
#include "MenuCubeSpr.h"

//Å×½ºÆ®

#include"Select.h"
#include "Title.h"
_bool CLogo::m_bStart = true;
CLogo::CLogo(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev), m_pLoading(nullptr)
{
}


CLogo::~CLogo()
{
}

HRESULT CLogo::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(Ready_Proto(), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);

	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);


	m_pLoading = CLoading::Create(m_pGraphicDev, LOADING_STAGE);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);
	
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	return S_OK;
}

_int CLogo::Update_Scene(const _float & fTimeDelta)
{
	


	int iExit = __super::Update_Scene(fTimeDelta);

	if (true == m_pLoading->Get_Finish())
	{
		if (Engine::Get_DIKeyState(DIK_UP) == Engine::KEYPRESS)
			m_bStart = true;
		if (Engine::Get_DIKeyState(DIK_DOWN) == Engine::KEYPRESS)
			m_bStart = false;

		if (GetAsyncKeyState(VK_RETURN)&&m_bStart==true)
		{
			CScene*	pScene = CStage1::Create(m_pGraphicDev);
			NULL_CHECK_RETURN(pScene, -1);

			FAILED_CHECK_RETURN(Engine::Set_Scene(pScene), E_FAIL);
			pScene->Update_Scene(fTimeDelta);
			return 0;
		}
		else if (GetAsyncKeyState(VK_RETURN) && m_bStart==false)
			PostQuitMessage(0);

	}

	return iExit;
}

void CLogo::LateUpdate_Scene(void)
{
	__super::LateUpdate_Scene();
}

void CLogo::Render_Scene(void)
{
	Engine::Render_Font(L"Font_Default", m_pLoading->Get_String(), &_vec2(20.f, 20.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));

}

HRESULT CLogo::Ready_Proto(void)
{
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"RcTex", CRcTex::Create(m_pGraphicDev)), E_FAIL);

	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"RcAlpha", CRcAlpha::Create(m_pGraphicDev)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"RcGradation", CRcGradation::Create(m_pGraphicDev)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Title_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/menuLogoSpr/menuLogoSpr.png")), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Spark_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/sparkSpr/SparkSpr_0%d.png", 10)), E_FAIL);
	
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"MenuCube_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/menuCubeSpr/menuCube.png")), E_FAIL);
	
	//FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Smoke_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/menuCubeSpr/menuSmoke.png")), E_FAIL);
	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Select_Texture", CTexture::Create(m_pGraphicDev, TEX_NORMAL, L"../Resource/Texture/Export_Textures/Sprites/menuLogoSpr/menuLogoSpr_%d.png", 11)), E_FAIL);

	FAILED_CHECK_RETURN(Engine::Ready_Proto(L"Transform", CTransform::Create(m_pGraphicDev)), E_FAIL);
	return S_OK;
}

HRESULT CLogo::Ready_Layer_Environment(const _tchar* pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	pGameObject = CLogoCamera::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"LogoCamera", pGameObject), E_FAIL);

	pGameObject = CBackgroundSpr::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BackGround", pGameObject), E_FAIL);

	for (size_t i = 0; i < 50; i++)
	{
		_float fRandX = rand() % WINCX - WINCX * 0.5f;
		_float fRandY = rand() % WINCY - WINCY * 0.5f;

		
		FAILED_CHECK_RETURN(FACTORY<CShiningStar>::Create(L"ShiningStar", pLayer, _vec3(fRandX, fRandY, 0.0f)), E_FAIL);
	}

	//FAILED_CHECK_RETURN(FACTORY<CMenuSmoke>::Create(L"MenuSmoke", pLayer, _vec3(0.0f, 0.0f, 0.0f)), E_FAIL);

	FAILED_CHECK_RETURN(FACTORY<CMenuCubeSpr>::Create(L"MenuCube", pLayer, _vec3(300.0f, -300.0f, 0.0f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CMenuCubeSpr>::Create(L"MenuCube", pLayer, _vec3(-300.0f, 200.0f, 0.0f)), E_FAIL);

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
	Safe_Release(m_pLoading);

	__super::Free();
}
