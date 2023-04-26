#include "stdafx.h"
#include "Bullet.h"

CBullet::CBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev),
	m_fSpeed(50.f)
{
}

CBullet::~CBullet()
{
}

HRESULT CBullet::Ready_GameObject(_vec3 & vPos, _vec3& vDir)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_vDir = vPos;
	m_pTex->Add_Anim(L"Idle", 0, 3, GetRandomFloat(1.f, 3.f), true);
	m_pTex->Switch_Anim(L"Idle");
	m_pTex->m_bUseFrameAnimation = true;
	return S_OK;
}

_int CBullet::Update_GameObject(const _float & fTimeDelta)
{
	if (m_pTransform->m_vInfo[INFO_POS].y >= 210.f)
		return OBJ_DEAD;
	Add_RenderGroup(RENDER_ALPHA, this);
	m_pTransform->m_vInfo[INFO_POS].y += m_fSpeed * fTimeDelta;
	m_pTex->Update_Anim(fTimeDelta);

	_matrix mat;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &mat);
	m_pTransform->Set_BillboardX(&mat);

	__super::Update_GameObject(fTimeDelta);
	return 0;
}

void CBullet::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CBullet::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTex->Set_Texture();
	m_pBuf->Render_Buffer();

	__super::Render_GameObject();
}

void CBullet::OnCollisionEnter(const Collision * collision)
{
}

HRESULT CBullet::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBuf = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBuf, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTex = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Bullet", this));
	NULL_CHECK_RETURN(m_pTex, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Bullet", pComponent });

	return S_OK;
}

CBullet * CBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos, _vec3& vDir)
{
	CBullet* pInstance = new CBullet(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(vPos, vDir)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CBullet::Free(void)
{
	__super::Free();
}
