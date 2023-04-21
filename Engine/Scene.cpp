#include "stdafx.h"
#include "Scene.h"
#include "Export_Function.h"
#include "..\Client\PreStage.h"
#include "..\Client\Key.h"
#include "..\Client\Fade.h"
#include "AbstractFactory.h"
CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev), m_eLoadingID(LOADING_END), m_pFade(nullptr), m_pPreScene(nullptr)
{
	// 귀찮아서 스택에 올렸다가 더귀찮아져버림;
	CKey::iKeyCnt = 0;
	m_pGraphicDev->AddRef();
}

CScene::~CScene()
{
}

CComponent * CScene::Get_Component(const _tchar * pLayerTag, const _tchar * pObjTag, const _tchar * pComponentTag, COMPONENTID eID)
{
	auto	iter = find_if(m_uMapLayer.begin(), m_uMapLayer.end(), CTag_Finder(pLayerTag));

	if (iter == m_uMapLayer.end())
		return nullptr;

	return iter->second->Get_Component(pObjTag, pComponentTag, eID);
}

CLayer * CScene::Get_Layer(const _tchar * pLayerTag)
{
	auto	iter = find_if(m_uMapLayer.begin(), m_uMapLayer.end(), CTag_Finder(pLayerTag));

	if (iter == m_uMapLayer.end())
		return nullptr;

	return iter->second;
}

CGameObject * CScene::Get_GameObject(const _tchar * pLayerTag, const _tchar * pObjTag)
{
	auto	iter = find_if(m_uMapLayer.begin(), m_uMapLayer.end(), CTag_Finder(pLayerTag));

	if (iter == m_uMapLayer.end())
		return nullptr;

	return iter->second->Get_GameObject(pObjTag);
}

_int CScene::Update_Scene(const _float & fTimeDelta)
{
	_int iResult = 0;
	_int iFadeFin = 0;
	if (m_pFade != nullptr)
	{
		// FADE UPDATE
		iFadeFin = m_pFade->Update_GameObject(fTimeDelta);
		m_pFade->Render_GameObject();
		// 페이드 인을 한 후에 프리스테이지를 불러 로딩을 실행.
		if (STAGE_FADEIN == iFadeFin)
		{
			m_pPreScene = CPreStage::Create(m_pGraphicDev, m_eLoadingID);
			Engine::Set_Scene(m_pPreScene);
			return 0;
		}
		else if (STAGE_FADEOUT == iFadeFin)
		{
			Safe_Release(m_pFade);
		}
	}
	for (auto& iter : m_uMapLayer)
	{
		iResult = iter.second->Update_Layer(fTimeDelta);

		if (STAGE_END == iResult)
		{
			// 현재 저장된 스테이지 아이디에서 다음 스테이지로 추가
			m_eLoadingID = LOADINGID((_int)m_eLoadingID + 1);
			CScene*	pScene = CPreStage::Create(m_pGraphicDev, m_eLoadingID);
			NULL_CHECK_RETURN(pScene, -1);

			Engine::Set_Scene(pScene);
			pScene->Update_Scene(fTimeDelta);
			return 0;
		}
		else if (STAGE_FAILED == iResult)
		{
			// 스테이지 실패
			if (m_pFade == nullptr)
			{
				m_eLoadingID = LOADINGID((_int)m_eLoadingID);
				m_pFade = CFade::Create(m_pGraphicDev, true);
			}
		}
		if (iResult & 0x80000000)
			return iResult;
	}
	return iResult;
}

void CScene::LateUpdate_Scene(void)
{
	for (auto& iter : m_uMapLayer)
		iter.second->LateUpdate_Layer();
}

void Engine::CScene::Free(void)
{
	for_each(m_uMapLayer.begin(), m_uMapLayer.end(), CDeleteMap());
	m_uMapLayer.clear();	
	Safe_Release(m_pGraphicDev);
}
