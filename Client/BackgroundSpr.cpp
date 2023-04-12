#include "stdafx.h"
#include "BackgroundSpr.h"

CBackgroundSpr::CBackgroundSpr(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CBackgroundSpr::~CBackgroundSpr()
{
}

HRESULT CBackgroundSpr::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale.x = WINCX*0.5f;
	m_pTransform->m_vScale.y = WINCY*0.5f;

	return S_OK;
}

_int CBackgroundSpr::Update_GameObject(const _float & fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_PRIORITY, this);

	return 0;
}

void CBackgroundSpr::Render_GameObject(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

HRESULT CBackgroundSpr::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcGradation*>(Engine::Clone_Proto(L"RcGradation", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcGradation", pComponent });

	return S_OK;
}

CBackgroundSpr * CBackgroundSpr::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBackgroundSpr*		pInstance = new CBackgroundSpr(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBackgroundSpr::Free(void)
{
	__super::Free();
}
