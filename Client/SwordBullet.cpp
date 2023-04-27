#include "stdafx.h"
#include "SwordBullet.h"

CSwordBullet::CSwordBullet(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBullet(pGraphicDev)
{
}

CSwordBullet::~CSwordBullet()
{
}

HRESULT CSwordBullet::Ready_Bullet(_vec3 & vPos, _vec3& vDir)
{
	__super::Ready_Bullet(vPos);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fSpeed = 80.f;
	m_pTransform->m_vScale = _vec3{ 10.f, 10.f, 10.f };
	m_pTex->Add_Anim(L"Idle", 0, 3, 1.f, true);
	m_pTex->Switch_Anim(L"Idle");
	m_pTex->m_bUseFrameAnimation = true;
	m_bShoot = false;
	Engine::Ready_Frame(L"Sword1Sec", 1.f);

	m_pCollider->Set_BoundingBox(_vec3{ 4.f, 4.f, 4.f });

	return S_OK;
}

void CSwordBullet::Ready_Pool(_vec3 & vPos, _vec3 & vDir)
{
	__super::Ready_Pool(vPos);
	m_fSpeed = 80.f;
	m_pTransform->m_vScale = _vec3{ 10.f, 10.f, 10.f };
	m_pTex->Add_Anim(L"Idle", 0, 3, 1.f, true);
	m_pTex->Switch_Anim(L"Idle");
	m_pTex->m_bUseFrameAnimation = true;
	m_bShoot = false;

	Engine::Ready_Frame(L"Sword1Sec", 1.f);

	m_pCollider->Set_BoundingBox(_vec3{ 4.f, 4.f, 4.f });
}

_int CSwordBullet::Update_GameObject(const _float & fTimeDelta)
{
	_int iResult = 0;
	m_pTransform->m_vAngle.y -= D3DXToRadian(15.f);
	if (Engine::IsPermit_Call(L"Sword1Sec", fTimeDelta) && false == m_bShoot)
		m_bShoot = true;
	if (m_bShoot)
		m_pTransform->m_vInfo[INFO_POS] += _vec3(0, 1, 0) * m_fSpeed * fTimeDelta;
		
	m_pTex->Update_Anim(fTimeDelta);

	iResult = __super::Update_GameObject(fTimeDelta);
	return iResult;
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
	pComponent = m_pTex = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"SwordBullet", this));
	NULL_CHECK_RETURN(m_pTex, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"SwordBullet", pComponent });

	return S_OK;
}

CSwordBullet * CSwordBullet::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CSwordBullet* pInstance = new CSwordBullet(pGraphicDev);
	if (FAILED(pInstance->Ready_Bullet(vPos)))
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
