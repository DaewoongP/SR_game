#include "stdafx.h"
#include "MenuSmoke.h"


CMenuSmoke::CMenuSmoke(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CMenuSmoke::~CMenuSmoke()
{
}

HRESULT CMenuSmoke::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { WINCX * 0.5f, WINCY * 0.5f, 1.f };

	m_pTransform->m_vInfo[INFO_POS] = vPos;

	return S_OK;
}

_int CMenuSmoke::Update_GameObject(const _float & fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CMenuSmoke::Render_GameObject(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

HRESULT CMenuSmoke::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcAlpha*>(Engine::Clone_Proto(L"RcAlpha", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcAlpha", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Smoke_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Smoke_Texture", pComponent });

	return S_OK;
}

CMenuSmoke * CMenuSmoke::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CMenuSmoke*		pInstance = new CMenuSmoke(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMenuSmoke::Free(void)
{
	__super::Free();
}