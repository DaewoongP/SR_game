#include "stdafx.h"
#include "Boss2Nose.h"

CBoss2Nose::CBoss2Nose(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBoss2Parts(pGraphicDev)
{
}

CBoss2Nose::~CBoss2Nose()
{
}

HRESULT CBoss2Nose::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTextureCom->Add_Anim(L"Idle", 0, 8, 2, true);
	m_pTextureCom->Add_Anim(L"Stump", 0, 5, 2, false);
	m_pTextureCom->Switch_Anim(L"Idle");
	m_pTransform->m_vScale = _vec3(0.5f, 0.5f, 0.5f);
	m_pTextureCom->m_bUseFrameAnimation = true;
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_OriginTexture = 0;
	m_WhiteTexture = 8;
	m_CurrentTexture = m_OriginTexture;
	return S_OK;
}

_int CBoss2Nose::Update_GameObject(const _float & fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	m_pTextureCom->Update_Anim(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

_int CBoss2Nose::Update_Too(const _float & fTimeDelta)
{
	return _int();
}

_int CBoss2Nose::Update_Top(const _float & fTimeDelta)
{
	return _int();
}

void CBoss2Nose::LateUpdate_GameObject(void)
{
}

void CBoss2Nose::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(m_CurrentTexture);
	//m_pShadow->Render_Shadow(m_pBufferCom);

	m_pBufferCom->Render_Buffer();
	__super::Render_GameObject();
}

void CBoss2Nose::Render_Too(void)
{
}

void CBoss2Nose::Render_Top(void)
{
}

HRESULT CBoss2Nose::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Boss2_Nose", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Texture", pComponent });

	pComponent = m_pShadow = dynamic_cast<CShadow*>(Engine::Clone_Proto(L"Shadow", this));
	NULL_CHECK_RETURN(m_pShadow, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Shadow", pComponent });

	return S_OK;
}

CBoss2Nose * CBoss2Nose::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CBoss2Nose*		pInstance = new CBoss2Nose(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss2Nose::Free(void)
{
	__super::Free();
}
