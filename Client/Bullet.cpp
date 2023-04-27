#include "stdafx.h"
#include "Bullet.h"

CBullet::CBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev), m_pBuf(nullptr), m_pTex(nullptr), m_pCollider(nullptr)
{
}

CBullet::~CBullet()
{
}

HRESULT CBullet::Ready_Bullet(_vec3 & vPos, _vec3& vDir)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pCollider->Set_Options({ 1.5f, 1.5f, 1.5f }, COL_OBJ, false);
	D3DXVec3Normalize(&m_vDir, &vDir);
	return S_OK;
}

void CBullet::Ready_Pool(_vec3 & vPos, _vec3 & vDir)
{
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pCollider->Set_Options({ 1.5f, 1.5f, 1.5f }, COL_OBJ, false);
	D3DXVec3Normalize(&m_vDir, &vDir);
}

_int CBullet::Update_GameObject(const _float & fTimeDelta)
{
	if (m_pTransform->m_vInfo[INFO_POS].z >= 150.f)
		return OBJ_DEAD;
	Add_RenderGroup(RENDER_ALPHA, this);
	
	__super::Update_GameObject(fTimeDelta);
	return 0;
}

void CBullet::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

HRESULT CBullet::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBuf = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBuf, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });

	return S_OK;
}

void CBullet::Free(void)
{
	__super::Free();
}
