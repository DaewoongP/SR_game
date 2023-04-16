#include "stdafx.h"
#include "Boss2Face.h"

CBoss2Face::CBoss2Face(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBoss2Parts(pGraphicDev)
{
}

CBoss2Face::~CBoss2Face()
{
}

HRESULT CBoss2Face::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTextureCom->Add_Anim(L"Idle", 0, 16, 2, true);
	m_pTextureCom->Add_Anim(L"Stump", 0, 8, 2, false);
	m_pTextureCom->Switch_Anim(L"Idle");
	m_pTextureCom->m_bUseFrameAnimation = true;
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_OriginTexture = 0;
	m_WhiteTexture = 16;
	m_CurrentTexture = m_OriginTexture;
	m_UseAnim = true;
	return S_OK;
}

_int CBoss2Face::Update_GameObject(const _float & fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	m_pTextureCom->Update_Anim(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

_int CBoss2Face::Update_Too(const _float & fTimeDelta)
{
	return _int();
}

_int CBoss2Face::Update_Top(const _float & fTimeDelta)
{
	return _int();
}

void CBoss2Face::LateUpdate_GameObject(void)
{
}

void CBoss2Face::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(m_CurrentTexture);
	m_pBufferCom->Render_Buffer();
	__super::Render_GameObject();
}

void CBoss2Face::Render_Too(void)
{
}

void CBoss2Face::Render_Top(void)
{
}

HRESULT CBoss2Face::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Boss2_Face", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Texture", pComponent });

	return S_OK;
}

CBoss2Face * CBoss2Face::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CBoss2Face*		pInstance = new CBoss2Face(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss2Face::Free(void)
{
	__super::Free();
}
