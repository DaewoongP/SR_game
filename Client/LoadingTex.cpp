#include "stdafx.h"
#include "LoadingTex.h"
#include "PreStage.h"

CLoadingTex::CLoadingTex(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CLoadingTex::~CLoadingTex()
{
}

HRESULT CLoadingTex::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pLoadingTex->Add_Anim(L"Idle", 0, 12, 1.f, true);
	m_pLoadingTex->Switch_Anim(L"Idle");
	m_pTransform->m_vScale.x = 250;
	m_pTransform->m_vScale.y = 125;
	m_pTransform->m_vInfo[INFO_POS] = {0.f, -200.f , 1.f};
	m_iLoadingPercent = 0;
	return S_OK;
}

_int CLoadingTex::Update_GameObject(const _float & fTimeDelta)
{
	CScene* pScene = Engine::Get_Scene();
	if (pScene == nullptr)
		return 0;
	m_iLoadingPercent = dynamic_cast<CPreStage*>(pScene)->Get_LoadingPercent();
	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	m_pLoadingTex->Update_Anim(fTimeDelta);
	return 0;
}

void CLoadingTex::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	
	m_pLoadingTex->Set_Texture(m_iLoadingPercent);
	m_pBufferCom->Render_Buffer();

	__super::Render_GameObject();
}

HRESULT CLoadingTex::Add_Component(void)
{
	CComponent*		pComponent = nullptr;
	
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex" , pComponent });

	pComponent = m_pLoadingTex = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Loading", this));
	NULL_CHECK_RETURN(m_pLoadingTex, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Loading", pComponent });

	return S_OK;
}

CLoadingTex * CLoadingTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLoadingTex*		pInstance = new CLoadingTex(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLoadingTex::Free(void)
{
	__super::Free();
}
