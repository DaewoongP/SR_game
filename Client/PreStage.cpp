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
#include "..\Engine\AbstractFactory.h"
#include "LoadingTex.h"
#include "Fade.h"
#include "ChapterTex.h"
#include "ChapterCloudTex.h"

CPreStage::CPreStage(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID)
	:CScene(pGraphicDev), m_pLoading(nullptr), m_eLoadingID(eID)
{
}


CPreStage::~CPreStage()
{
}

HRESULT CPreStage::Ready_Scene(void)
{
	Engine::Set_Scene(this);
	m_pLoading = CLoading::Create(m_pGraphicDev, m_eLoadingID);
	NULL_CHECK_RETURN(m_pLoading, E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);
	if (nullptr != Engine::Get_Scene()) { Engine::Get_Scene()->Release_Fade(); }
	
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	return S_OK;
}

_int CPreStage::Update_Scene(const _float & fTimeDelta)
{
	// 스테이지 이동전 돌 수 있는 업데이트 ~ 렌더 씬
	int iExit = __super::Update_Scene(fTimeDelta);
	if (m_pLoading == nullptr)
		return 0;
	if (true == m_pLoading->Get_Finish() && Engine::IsPermit_Call(L"1Sec",fTimeDelta))
	{

		g_Is2D = true;
		g_IsInvin = false;
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

	FAILED_CHECK_RETURN(FACTORY<CLoadingTex>::Create(L"LoadingTex", pLayer), E_FAIL);
	
	FAILED_CHECK_RETURN(FACTORY<CChapterTex>::Create(L"ChapterTex", pLayer), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CChapterCloudTex>::Create(L"ChapterCloudTex", pLayer), E_FAIL);

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
