#include "stdafx.h"
#include "Boss2Foot.h"

CBoss2Foot::CBoss2Foot(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBoss2Parts(pGraphicDev)
{
}

CBoss2Foot::~CBoss2Foot()
{
}

HRESULT CBoss2Foot::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vScale = _vec3(1, 0.5f, 1);
	m_pTextureCom->Add_Anim(L"Idle", 7,9, 1, true);
	m_pTextureCom->Add_Anim(L"Paper", 0, 1, 1, true);
	m_pTextureCom->Add_Anim(L"Jump", 0, 7, 1, false);
	m_pTextureCom->Switch_Anim(L"Idle");
	m_pTextureCom->m_bUseFrameAnimation = true;
	m_OriginTexture = 0;
	m_WhiteTexture = 12;
	m_UseAnim = true;
	m_CurrentTexture = m_OriginTexture;
	return S_OK;
}

_int CBoss2Foot::Update_GameObject(const _float & fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	m_pTextureCom->Update_Anim(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

_int CBoss2Foot::Update_Too(const _float & fTimeDelta)
{
	return _int();
}

_int CBoss2Foot::Update_Top(const _float & fTimeDelta)
{
	return _int();
}

void CBoss2Foot::LateUpdate_GameObject(void)
{
}

void CBoss2Foot::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(m_CurrentTexture);
	m_pShadow->Render_Shadow(m_pBufferCom);

	m_pBufferCom->Render_Buffer();
	__super::Render_GameObject();
}

void CBoss2Foot::Render_Too(void)
{
}

void CBoss2Foot::Render_Top(void)
{
}

HRESULT CBoss2Foot::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Boss2_Foot", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Texture", pComponent });

	pComponent = m_pShadow = dynamic_cast<CShadow*>(Engine::Clone_Proto(L"Shadow", this));
	NULL_CHECK_RETURN(m_pShadow, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Shadow", pComponent });

	return S_OK;
}

CBoss2Foot * CBoss2Foot::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CBoss2Foot*		pInstance = new CBoss2Foot(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss2Foot::Free(void)
{
	__super::Free();
}
