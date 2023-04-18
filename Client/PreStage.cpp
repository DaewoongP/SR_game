#include "stdafx.h"
#include "PreStage.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Stage3.h"
#include "Export_Function.h"
#include "UICamera.h"
#include "Title.h"
#include "Logo.h"
#include "BackgroundSpr.h"
#include "StageCamera.h"
#include "LogoCamera.h"

CPreStage::CPreStage(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
	:CScene(pGraphicDev), m_pLoading(nullptr), m_eLoadingID(eID)
{
}


CPreStage::~CPreStage()
{
}

HRESULT CPreStage::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);
	m_pLoading = CLoading::Create(m_pGraphicDev, m_eLoadingID);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	return S_OK;
}

_int CPreStage::Update_Scene(const _float & fTimeDelta)
{
	// 스테이지 이동전 돌 수 있는 업데이트 ~ 렌더 씬
	int iExit = __super::Update_Scene(fTimeDelta);
	if (Engine::Get_DIKeyState(DIK_SPACE) == Engine::KEYDOWN && true == m_pLoading->Get_Finish())
	{
		g_Is2D = true;
		CScene* pScene = m_pScene;
		Engine::Set_Scene(pScene);

		pScene->Update_Scene(fTimeDelta);
		return 0;
	}
	return iExit;
}

void CPreStage::LateUpdate_Scene(void)
{
}

void CPreStage::Render_Scene(void)
{
	Engine::Render_Font(L"Font_Default", m_pLoading->Get_String(), &_vec2(WINCX * 0.4f, WINCY * 0.7f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

HRESULT CPreStage::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	pGameObject = CUICamera::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"UICamera", pGameObject), E_FAIL);

	pGameObject = CBackgroundSpr::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"BackGround", pGameObject), E_FAIL);

	m_uMapLayer.insert({ pLayerTag, pLayer });
	return S_OK;
}

CPreStage * CPreStage::Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
{
	CPreStage *	pInstance = new CPreStage(pGraphicDev, eID);
	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPreStage::Free(void)
{
	Safe_Release(m_pLoading);

	__super::Free();
}
