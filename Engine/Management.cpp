#include "stdafx.h"
#include "Export_Utility.h"

IMPLEMENT_SINGLETON(CManagement)

CManagement::CManagement()
{
}

CManagement::~CManagement()
{
	Free();
}

CComponent * CManagement::Get_Component(const _tchar * pLayerTag, const _tchar * pObjTag, const _tchar * pComponentTag, COMPONENTID eID)
{
	NULL_CHECK_RETURN(m_pScene, nullptr);
	return m_pScene->Get_Component(pLayerTag, pObjTag, pComponentTag, eID);
}

CLayer * CManagement::Get_Layer(const _tchar * pLayerTag)
{
	NULL_CHECK_RETURN(m_pScene, nullptr);
	return m_pScene->Get_Layer(pLayerTag);
}

HRESULT CManagement::Set_Scene(CScene * pScene)
{
	Safe_Release(m_pScene);
	Engine::Clear_RenderGroup();

	m_pScene = pScene;

	return S_OK;
}

_int CManagement::Update_Management(const _float & fTimeDelta)
{
	NULL_CHECK_RETURN(m_pScene, -1);
	
	return m_pScene->Update_Scene(fTimeDelta);
}

void CManagement::LateUpdate_Management()
{
	NULL_CHECK(m_pScene);
	m_pScene->LateUpdate_Scene();
	// 충돌체크
	Engine::Check_Collision(COL_OBJ, COL_OBJ);
	Engine::Check_Collision(COL_OBJ, COL_ENV);
}

void CManagement::Render_Management(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Engine::Render_GameObject(pGraphicDev);

	NULL_CHECK(m_pScene);
	m_pScene->Render_Scene();
}

void CManagement::Free(void)
{
	Safe_Release(m_pScene);
}
