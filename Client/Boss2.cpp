#include "stdafx.h"
#include "Boss2.h"
#include "Export_Function.h"

CBoss2::CBoss2(LPDIRECT3DDEVICE9 pGraphicDev) 
	: CGameObject(pGraphicDev)
{
}


CBoss2::~CBoss2()
{
}

HRESULT CBoss2::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { 3.f, 3.f, 3.f };
	m_pCollider->Set_BoundingBox(_vec3(4, 4, 2));
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pRigid->m_bUseGrivaty = false;

	return S_OK;
}

_int CBoss2::Update_GameObject(const _float & fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	return 0;
}

_int CBoss2::Update_Too(const _float & fTimeDelta)
{
	return 0;
}

_int CBoss2::Update_Top(const _float & fTimeDelta)
{
	return 0;
}

void CBoss2::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CBoss2::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();
	__super::Render_GameObject();
}

void CBoss2::Render_Too(void)
{
	int a = 0;
}

void CBoss2::Render_Top(void)
{
	int a = 0;
}

void CBoss2::SwapTrigger()
{
}

void CBoss2::OnCollisionEnter(const Collision * collision)
{
	__super::OnCollisionEnter(collision);
}

void CBoss2::OnCollisionStay(const Collision * collision)
{
	__super::OnCollisionStay(collision);
}

void CBoss2::OnCollisionExit(const Collision * collision)
{
	__super::OnCollisionExit(collision);
}

HRESULT CBoss2::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Boss2_Head", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Texture", pComponent });

	pComponent = m_pRigid = dynamic_cast<CRigidbody*>(Engine::Clone_Proto(L"Rigidbody", this));
	NULL_CHECK_RETURN(m_pRigid, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Rigidbody", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });

	return S_OK;
}

CBoss2 * CBoss2::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CBoss2*		pInstance = new CBoss2(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss2::Free(void)
{
	__super::Free();
}
