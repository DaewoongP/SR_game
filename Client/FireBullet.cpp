#include "stdafx.h"
#include "FireBullet.h"

CFireBullet::CFireBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBullet(pGraphicDev)
{
}

CFireBullet::~CFireBullet()
{
}

HRESULT CFireBullet::Ready_Bullet(_vec3& vPos, _vec3& vDir)
{
	__super::Ready_Bullet(vPos);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fSpeed = 50.f;
	m_pTransform->m_vAngle.y = D3DXToRadian(90);
	m_pTex->Add_Anim(L"Idle", 0, 2, 1.f, true);
	m_pTex->Switch_Anim(L"Idle");
	m_pTex->m_bUseFrameAnimation = true;
	vDir = { GetRandomFloat(-1.f, 1.f), 20.f, GetRandomFloat(-1.f, 1.f) };
	D3DXVec3Normalize(&m_vDir, &vDir);
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

	pComponent = m_pTex = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"FireBullet", this));
	NULL_CHECK_RETURN(m_pTex, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"FireBullet", pComponent });

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
