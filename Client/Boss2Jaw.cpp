#include "stdafx.h"
#include "Boss2Jaw.h"

CBoss2Jaw::CBoss2Jaw(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBoss2Parts(pGraphicDev)
{
}

CBoss2Jaw::~CBoss2Jaw()
{
}

HRESULT CBoss2Jaw::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTextureCom->Add_Anim(L"Idle", 0, 16, 2, true);
	m_pTextureCom->Add_Anim(L"Stump", 0, 9, 2, false);
	m_pTextureCom->Switch_Anim(L"Idle");
	m_pTransform->m_vScale = _vec3(1.5f, 2, 2);
	m_pTextureCom->m_bUseFrameAnimation = true;
	m_OriginTexture = 0;
	m_WhiteTexture = 16;
	m_CurrentTexture = m_OriginTexture;
	return S_OK;
}

_int CBoss2Jaw::Update_GameObject(const _float & fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	m_pTextureCom->Update_Anim(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

_int CBoss2Jaw::Update_Too(const _float & fTimeDelta)
{
	return _int();
}

_int CBoss2Jaw::Update_Top(const _float & fTimeDelta)
{
	return _int();
}

void CBoss2Jaw::LateUpdate_GameObject(void)
{
}

void CBoss2Jaw::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(m_CurrentTexture);
	m_pShadow->Render_Shadow(m_pBufferCom);

	m_pBufferCom->Render_Buffer();
	__super::Render_GameObject();
}

void CBoss2Jaw::Render_Too(void)
{
}

void CBoss2Jaw::Render_Top(void)
{
}

HRESULT CBoss2Jaw::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Boss2_Jaw", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Texture", pComponent });

	pComponent = m_pShadow = dynamic_cast<CShadow*>(Engine::Clone_Proto(L"Shadow", this));
	NULL_CHECK_RETURN(m_pShadow, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Shadow", pComponent });

	return S_OK;
}

CBoss2Jaw * CBoss2Jaw::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CBoss2Jaw*		pInstance = new CBoss2Jaw(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss2Jaw::Free(void)
{
	__super::Free();
}
