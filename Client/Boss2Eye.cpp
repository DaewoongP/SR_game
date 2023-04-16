#include "stdafx.h"
#include "Boss2Eye.h"

CBoss2Eye::CBoss2Eye(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBoss2Parts(pGraphicDev)
{
}

CBoss2Eye::~CBoss2Eye()
{
}

HRESULT CBoss2Eye::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_CurrentTexture = m_OriginTexture;
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vScale = _vec3(0.7f, 0.7f, 0.7f);
	m_pTransform->m_vScale = _vec3(2.0f, 1.5f, 2.0f);
	m_OriginTexture = 0;
	m_WhiteTexture = 1;
	m_CurrentTexture = m_OriginTexture;
	return S_OK;
}

_int CBoss2Eye::Update_GameObject(const _float & fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

_int CBoss2Eye::Update_Too(const _float & fTimeDelta)
{
	return _int();
}

_int CBoss2Eye::Update_Top(const _float & fTimeDelta)
{
	return _int();
}

void CBoss2Eye::LateUpdate_GameObject(void)
{
}

void CBoss2Eye::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(m_CurrentTexture);
	//m_pShadow->Render_Shadow(m_pBufferCom);

	m_pBufferCom->Render_Buffer();
	__super::Render_GameObject();
}

void CBoss2Eye::Render_Too(void)
{
}

void CBoss2Eye::Render_Top(void)
{
}

HRESULT CBoss2Eye::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Boss2_Eye", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Texture", pComponent });

	pComponent = m_pShadow = dynamic_cast<CShadow*>(Engine::Clone_Proto(L"Shadow", this));
	NULL_CHECK_RETURN(m_pShadow, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Shadow", pComponent });


	return S_OK;
}

CBoss2Eye * CBoss2Eye::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CBoss2Eye*		pInstance = new CBoss2Eye(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss2Eye::Free(void)
{
	__super::Free();
}
