#include "stdafx.h"
#include "Boss2EyeBrow.h"

CBoss2EyeBrow::CBoss2EyeBrow(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CBoss2EyeBrow::~CBoss2EyeBrow()
{
}

HRESULT CBoss2EyeBrow::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	return S_OK;
}

_int CBoss2EyeBrow::Update_GameObject(const _float & fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

_int CBoss2EyeBrow::Update_Too(const _float & fTimeDelta)
{
	return _int();
}

_int CBoss2EyeBrow::Update_Top(const _float & fTimeDelta)
{
	return _int();
}

void CBoss2EyeBrow::LateUpdate_GameObject(void)
{
}

void CBoss2EyeBrow::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();
	__super::Render_GameObject();
}

void CBoss2EyeBrow::Render_Too(void)
{
}

void CBoss2EyeBrow::Render_Top(void)
{
}

HRESULT CBoss2EyeBrow::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Boss2_EyeBrow", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Texture", pComponent });

	return S_OK;
}

CBoss2EyeBrow * CBoss2EyeBrow::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CBoss2EyeBrow*		pInstance = new CBoss2EyeBrow(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss2EyeBrow::Free(void)
{
	__super::Free();
}
