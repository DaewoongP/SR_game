#include "stdafx.h"
#include "Title.h"

#include"..\Engine\Export_Function.h"
CTitle::CTitle(LPDIRECT3DDEVICE9 pGraphicDev):CGameObject(pGraphicDev)
{
}

CTitle::~CTitle()
{
}

HRESULT CTitle::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS].y = 200.f;
	m_pTransform->m_vScale = { 256.f,128.f,1.f };

	return S_OK;
}

_int CTitle::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return S_OK;

}

void CTitle::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();

}

void CTitle::Render_GameObject(void)
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

HRESULT CTitle::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"RcTex", pComponent });


	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Title_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Texture", pComponent });

	return S_OK;
}

CTitle* CTitle::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTitle* pInstance = new CTitle(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CTitle::Free(void)
{
	__super::Free();
}
