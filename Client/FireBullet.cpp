#include "stdafx.h"
#include "FireBullet.h"

CFireBullet::CFireBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev),
	m_fSpeed(50.f)
{
}

CFireBullet::~CFireBullet()
{
}

HRESULT CFireBullet::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pCollider->Set_Options({ 1.f, 1.f, 1.f }, COL_OBJ, false);
	m_pTex->Add_Anim(L"Idle", 0, 2, 1.f, true);
	m_pTex->Switch_Anim(L"Idle");
	m_pTex->m_bUseFrameAnimation = true;
	GetRandomVectorIncircle(&m_vDir, 1.f);
	return S_OK;
}

_int CFireBullet::Update_GameObject(const _float & fTimeDelta)
{
	if (m_pTransform->m_vInfo[INFO_POS].y >= 210.f)
		return OBJ_DEAD;
	Add_RenderGroup(RENDER_ALPHA, this);
	m_pTransform->m_vInfo[INFO_POS] += m_vDir * m_fSpeed * fTimeDelta;
	m_pTex->Update_Anim(fTimeDelta);

	_matrix mat;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &mat);
	m_pTransform->Set_BillboardX(&mat);

	__super::Update_GameObject(fTimeDelta);
	return 0;
}

void CFireBullet::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CFireBullet::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTex->Set_Texture();
	m_pBuf->Render_Buffer();

	__super::Render_GameObject();
}

void CFireBullet::OnCollisionEnter(const Collision * collision)
{
}

HRESULT CFireBullet::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBuf = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBuf, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTex = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Bullet", this));
	NULL_CHECK_RETURN(m_pTex, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Bullet", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });

	return S_OK;
}

CFireBullet * CFireBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CFireBullet* pInstance = new CFireBullet(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CFireBullet::Free(void)
{
	__super::Free();
}
