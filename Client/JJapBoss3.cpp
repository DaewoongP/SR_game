#include "stdafx.h"
#include "JJapBoss3.h"
#include "Export_Function.h"
#include "StageCamera.h"
#include "AbstractFactory.h"

#include "Boss3Hand.h"
#include "Boss3HandPart.h"
#include "Boss3Eye.h"
#include "Boss3EyePupil.h"
#include "Boss3Eyebrow.h"
#include "Boss3Mouth.h"

JJapBoss3::JJapBoss3(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCube(pGraphicDev),
	m_fSpeed(25.f), m_fXAngle(0.f), m_fCoolDown(0.f), m_fAttackCoolDown(0.f),
	m_fShootCoolDown(0.f), m_fPreToo(0.f), m_fPreTop(0.f), m_fShockDown(0.f),
	m_iBossHp(3),
	m_bCreateHand(true)
{
	m_pBossLeft = nullptr;
	m_pBossRight = nullptr;
}

JJapBoss3::~JJapBoss3()
{
}

HRESULT JJapBoss3::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = _vec3{ vPos.x, vPos.y, 10.f };
	m_pTransform->m_vScale = { 3.5f, 3.5f, 3.5f };
	m_pTransform->Rotation(ROT_Y, D3DXToRadian(-45.f));
	m_pTransform->m_bIsStatic = true;
	m_pCollider->Set_BoundingBox({ 7.f, 7.f, 7.f });
	m_pCollider->Set_Group(COL_OBJ);

	m_pShadowCom->m_fShadowHeight = 13.0f;

	return S_OK;
}

_int JJapBoss3::Update_Too(const _float & fTimeDelta)
{
	return _int();
}

_int JJapBoss3::Update_Top(const _float & fTimeDelta)
{
	return _int();
}

_int JJapBoss3::Update_GameObject(const _float & fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void JJapBoss3::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}
void JJapBoss3::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	__super::Render_GameObject();
}

void JJapBoss3::OnCollisionEnter(const Collision * collision)
{
	__super::OnCollisionEnter(collision);
}

void JJapBoss3::OnCollisionStay(const Collision * collision)
{
	__super::OnCollisionStay(collision);
}

void JJapBoss3::OnCollisionExit(const Collision * collision)
{
	__super::OnCollisionExit(collision);
}

void JJapBoss3::SwapTrigger()
{
}

void JJapBoss3::MakeChain()
{
}

HRESULT JJapBoss3::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"CubeTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"CubeTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Stage3_Boss_Cube", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Stage3_Boss_Cube", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });

	pComponent = m_pShadowCom = dynamic_cast<CShadow*>(Engine::Clone_Proto(L"Shadow", this));
	NULL_CHECK_RETURN(m_pShadowCom, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Shadow",pComponent });

	pComponent = m_pLandingParticle = dynamic_cast<CCircularParticle*>(Engine::Clone_Proto(L"Boss2LandParticle", this));
	NULL_CHECK_RETURN(m_pLandingParticle, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"BossLandingParticle",pComponent });

	pComponent = m_pScreamParticle = dynamic_cast<CTexParticle*>(Engine::Clone_Proto(L"BossScream", this));
	NULL_CHECK_RETURN(m_pScreamParticle, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"BossScream", pComponent });

	return S_OK;
}

void JJapBoss3::LookAtPlayer()
{
}

void JJapBoss3::FollowPlayer(const _float & fTimeDelta)
{
}

void JJapBoss3::BossAttack(const _float & fTimeDelta)
{
}

void JJapBoss3::ShootBullet(const _float & fTimeDelta)
{
}

void JJapBoss3::Do_Scream(const _float & fTimeDelta)
{
}

void JJapBoss3::End_Scream(const _float & fTimeDelta)
{
}

JJapBoss3 * JJapBoss3::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	JJapBoss3* pInstance = new JJapBoss3(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void JJapBoss3::Free(void)
{
	__super::Free();
}