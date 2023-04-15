#include "stdafx.h"
#include "PreStage.h"
#include "Stage1.h"
#include "Export_Function.h"
#include "UICamera.h"
#include "Title.h"
#include "Stage2.h"

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
	int iExit = __super::Update_Scene(fTimeDelta);
	if (true == m_pLoading->Get_Finish())
	{
		if (Engine::Get_DIKeyState(DIK_RETURN) == Engine::KEYDOWN)
		{
			CScene*	pScene = nullptr;
			switch (m_eLoadingID)
			{
			case LOADING_LOGO:
				break;
			case LOADING_STAGE1:
				pScene = CStage1::Create(m_pGraphicDev);
				break;
			case LOADING_STAGE2:
				pScene = CStage2::Create(m_pGraphicDev);
				break;
			case LOADING_BOSS:
				break;
			case LOADING_END:
				break;
			}

			NULL_CHECK_RETURN(pScene, -1);

			FAILED_CHECK_RETURN(Engine::Set_Scene(pScene), E_FAIL);
			pScene->Update_Scene(fTimeDelta);
			return 0;
		}
	}
	return iExit;
}

void CPreStage::LateUpdate_Scene(void)
{
}

void CPreStage::Render_Scene(void)
{
	Engine::Render_Font(L"Font_Default", m_pLoading->Get_String(), &_vec2(20.f, 20.f), D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
}

HRESULT CPreStage::Ready_Layer_UI(const _tchar * pLayerTag)
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
