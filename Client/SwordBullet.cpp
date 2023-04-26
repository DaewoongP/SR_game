#include "stdafx.h"
#include "SwordBullet.h"

CSwordBullet::CSwordBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev),
	m_fSpeed(80.f)
{
}

CSwordBullet::~CSwordBullet()
{
}

HRESULT CSwordBullet::Ready_GameObject(_vec3 & vPos, _vec3& vInitPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vScale *= 10.f;
	m_pTransform->m_vInfo[INFO_POS] = vPos + vInitPos;
	m_pCollider->Set_Options({ 1.f, 1.f, 1.f }, COL_OBJ, false);
	
	m_pTex->Add_Anim(L"Idle", 0, 3, 1.f, true);
	m_pTex->Switch_Anim(L"Idle");
	m_pTex->m_bUseFrameAnimation = true;
	m_bShoot = false;
	Engine::Ready_Frame(L"Sword1Sec", 1.f);
	return S_OK;
}

_int CSwordBullet::Update_GameObject(const _float & fTimeDelta)
{
	if (m_pTransform->m_vInfo[INFO_POS].y >= 210.f)
		return OBJ_DEAD;
	m_pTransform->m_vAngle.y -= D3DXToRadian(10.f);
	Add_RenderGroup(RENDER_ALPHA, this);
	if (Engine::IsPermit_Call(L"Sword1Sec", fTimeDelta) && false == m_bShoot)
	{
		m_bShoot = true;
	}

	if (m_bShoot)
	{
		m_pTransform->m_vInfo[INFO_POS] += _vec3(0, 1, 0) * m_fSpeed * fTimeDelta;
	}
		
	m_pTex->Update_Anim(fTimeDelta);

	__super::Update_GameObject(fTimeDelta);
	return 0;
}

void CSwordBullet::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CSwordBullet::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTex->Set_Texture();
	m_pBuf->Render_Buffer();

	__super::Render_GameObject();
}

void CSwordBullet::OnCollisionEnter(const Collision * collision)
{
}

HRESULT CSwordBullet::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBuf = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBuf, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTex = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"SwordBullet", this));
	NULL_CHECK_RETURN(m_pTex, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"SwordBullet", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });

	return S_OK;
}

CSwordBullet * CSwordBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos, _vec3& vInitPos)
{
	CSwordBullet* pInstance = new CSwordBullet(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(vPos, vInitPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CSwordBullet::Free(void)
{
	__super::Free();
}
