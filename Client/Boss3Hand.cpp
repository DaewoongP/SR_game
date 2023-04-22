#include "stdafx.h"
#include"Boss3.h"
#include "Boss3Hand.h"
#include"..\Client\Toodee.h"
#include"..\Client\Topdee.h"
#include"StageCamera.h"
#include "Export_Function.h"

CBoss3Hand::CBoss3Hand(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCube(pGraphicDev),
	m_fSpeed(27.f), m_fCoolDown(0.f), m_fAttackCoolDown(0.f), 
	m_fIdleCycle(0.f), m_fIdleAngle(0.f),
	m_fLerpDist(0.f),
	m_iIndex(0), 
	m_bAttack(false), m_bIdleMove(true), m_bIdleStop(false), m_bLerpMove(false)
{
}

CBoss3Hand::~CBoss3Hand()
{
}

HRESULT CBoss3Hand::Ready_GameObject(_vec3 & vPos, _int iIndex)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vInfo[INFO_POS].z = 9.f;
	m_pTransform->Rotation(ROT_Y, D3DXToRadian(-65.f));
	m_pTransform->m_vScale = { 2.f, 2.f, 2.f };
	m_pTransform->m_bIsStatic = true;
	m_pCollider->Set_BoundingBox({ 4.f, 4.f, 4.f });
	m_pCollider->Set_Group(COL_OBJ);

	m_iIndex = iIndex;

	m_pShadowCom->m_fShadowHeight = 12.f;

	return S_OK;
}

_int CBoss3Hand::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bLerpMove)
		Lerp_Moving(fTimeDelta);

	if (m_bDead)
		return OBJ_DEAD;
    
	if (m_bShock)
	{
		if (m_fShockCollDown == 0.f)
		{
			// 3보스용 옵션값
			BoundingBox Testbox;
			_vec3 vPos = m_pTransform->m_vInfo[INFO_POS];
			vPos.z -= 2.f;
			Testbox.Offset(vPos);
			m_pSparkParticle->Set_BoundingBox(Testbox);
			m_pSparkParticle->Set_RandomGen(3.f);
			m_pSparkParticle->Set_SizeLifeTime(1.f);
			m_pSparkParticle->Set_Options(1.2f, 15.f);
			m_pElecParticle->Set_BoundingBox(Testbox);
			m_pElecParticle->Set_RandomGen(3.f);
			m_pElecParticle->Set_SizeLifeTime(1.f);
			m_pElecParticle->Set_Options(1.2f, 15.f);
			m_pSparkParticle->Start_Particle();
			m_pElecParticle->Start_Particle();
		}
		m_fShockCollDown += fTimeDelta;

	}
	if (m_fShockCollDown > 3.f)
	{
		m_bShock = false;
		m_fShockCollDown = 0.f;
	}
	
   IdleMove(fTimeDelta);
	
	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

_int CBoss3Hand::Update_Too(const _float & fTimeDelta)
{
	m_pTransform->m_vInfo[INFO_POS].z = 9.f;

	if (m_pTransform->m_vAngle.x != D3DXToRadian(0.f))
	{
		m_fTimer += fTimeDelta * 4.0f;

		if (1.0f <= m_fTimer)
		{
			m_fTimer = 1.0f;
		}

		m_pTransform->m_vAngle.x = Lerp(D3DXToRadian(-80.f), D3DXToRadian(0.f), m_fTimer);

		if (1.0f == m_fTimer)
		{
			m_fTimer = 0.0f;
		}
	}

	CGameObject::Update_Too(fTimeDelta);

	return 0;
}

_int CBoss3Hand::Update_Top(const _float & fTimeDelta)
{
	if(!m_pTransform->m_vInfo[INFO_POS].z == 3.f)
	{
		m_pTransform->m_vInfo[INFO_POS].z -= 1.f;
	}

	if (m_pTransform->m_vAngle.x != D3DXToRadian(-80.f))
	{
		m_fTimer += fTimeDelta * 4.0f;

		if (1.0f <= m_fTimer)
		{
			m_fTimer = 1.0f;
		}

		m_pTransform->m_vAngle.x = Lerp(D3DXToRadian(0.f), D3DXToRadian(-80.f), m_fTimer);

		if (1.0f == m_fTimer)
		{
			m_fTimer = 0.0f;
		}
	}

	if (m_bAttack)
		FollowPlayer(fTimeDelta);

	CGameObject::Update_Top(fTimeDelta);

	return 0;
}

void CBoss3Hand::LateUpdate_GameObject(void)
{
	CGameObject::LateUpdate_GameObject();
}

void CBoss3Hand::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	if (m_bShock == true)
		m_pTextureCom->Set_Texture();

	else
		m_pTextureCom2->Set_Texture(m_iIndex);

	if(!g_Is2D)
		m_pShadowCom->Render_Shadow(m_pBufferCom);

	if (m_bLerpMove)
	{
		if (0.f < m_fLerpDist && 0.02f > m_fLerpDist)
			m_pTextureCom2->Set_Texture(m_iIndex);

		else if (0.02f < m_fLerpDist && 0.04f > m_fLerpDist)
			m_pTextureCom->Set_Texture();

		else if (0.04f < m_fLerpDist && 0.06f > m_fLerpDist)
			m_pTextureCom2->Set_Texture(m_iIndex);

		else if (0.06f < m_fLerpDist && 0.08f > m_fLerpDist)
			m_pTextureCom->Set_Texture();

		else if (0.08f < m_fLerpDist && 0.1f > m_fLerpDist)
		{
			m_bLerpMove = false;
			m_fLerpDist = 0.f;
		}
	}

	m_pBufferCom->Render_Buffer();

	CGameObject::Render_GameObject();

	m_pLandingParticle->Update_Particle();
	m_pSparkParticle->Update_Particle();
	m_pElecParticle->Update_Particle();
}

void CBoss3Hand::OnCollisionEnter(const Collision * collision)
{
	if (!lstrcmp(collision->otherObj->m_pTag, L"Toodee") && m_bShock == true)
	{
		CGameObject* pGameObject = Engine::Get_GameObject(L"Layer_GameLogic", L"Toodee");
		dynamic_cast<CToodee*>(pGameObject)->Set_AnimDead(); 
	}

	if (!lstrcmp(collision->otherObj->m_pTag, L"Topdee") && m_bShock == true)
	{
		CGameObject* pGameObject = Engine::Get_GameObject(L"Layer_GameLogic", L"Topdee");
	}
	__super::OnCollisionEnter(collision);
}

void CBoss3Hand::OnCollisionStay(const Collision * collision)
{
	CGameObject::OnCollisionStay(collision);
}

void CBoss3Hand::OnCollisionExit(const Collision * collision)
{
	CGameObject::OnCollisionExit(collision);
}

HRESULT CBoss3Hand::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"CubeTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"CubeTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Stage3_Boss_Hand_Blank_Cube", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Stage3_Boss_Hand_Blank_Cube", pComponent });

	pComponent = m_pTextureCom2 = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Stage3_Boss_Hand_Cube", this));
	NULL_CHECK_RETURN(m_pTextureCom2, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Stage3_Boss_Hand_Cube", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });

	pComponent = m_pShadowCom = dynamic_cast<CShadow*>(Engine::Clone_Proto(L"Shadow", this));
	NULL_CHECK_RETURN(m_pShadowCom, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Shadow",pComponent });

	pComponent = m_pLandingParticle = dynamic_cast<CCircularParticle*>(Engine::Clone_Proto(L"Boss2LandParticle", this));
	NULL_CHECK_RETURN(m_pLandingParticle, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"LandingParticle",pComponent });

	pComponent = m_pSparkParticle = dynamic_cast<CCircularParticle*>(Engine::Clone_Proto(L"SparkCircularParticle", this));
	NULL_CHECK_RETURN(m_pSparkParticle, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"SparkCircularParticle",pComponent });

	pComponent = m_pElecParticle = dynamic_cast<CCircularParticle*>(Engine::Clone_Proto(L"ElectrictCircularParticle", this));
	NULL_CHECK_RETURN(m_pElecParticle, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"ElectrictCircularParticle",pComponent });

	return S_OK;
}

void CBoss3Hand::Lerp_Moving(const _float & fTimeDelta)
{
	_vec3 vLerp;
	_vec3 vGoal;
	
	if(!lstrcmp(m_pTag, L"Boss3Left"))
		vGoal = _vec3(15.f, 20.f, 10.f);

	if (!lstrcmp(m_pTag, L"Boss3Right"))
		vGoal = _vec3(45.f, 20.f, 10.f);

	D3DXVec3Lerp(&vLerp, &m_pTransform->m_vInfo[INFO_POS], &vGoal, m_fLerpDist);
	m_pTransform->m_vInfo[INFO_POS].x = vLerp.x;
	m_pTransform->m_vInfo[INFO_POS].y = vLerp.y;

	m_fLerpDist += 0.001f;
}

void CBoss3Hand::FollowPlayer(const _float & fTimeDelta)
{
	if (m_bLerpMove)
		return;

	m_fCoolDown += fTimeDelta;

	CGameObject* pGameObject = Engine::Get_GameObject(L"Layer_GameLogic", L"Topdee");
	NULL_CHECK_RETURN(pGameObject, );

	// 추격을 진행하고
	if (2.f < m_fCoolDown && 3.5f > m_fCoolDown)
	{
		m_pTransform->Chase_Target(&pGameObject->m_pTransform->m_vInfo[INFO_POS], m_fSpeed, fTimeDelta);

		m_pTransform->m_vInfo[INFO_POS].z -= 31.f*fTimeDelta;
	}

	else if (4.f < m_fCoolDown )
	{
		if (m_bSpin)
		{
			StopSound(SOUND_EFFECT_ENEMY);
			PlaySound_Effect(L"79.wav", SOUND_EFFECT_ENEMY, 1.f);
			m_bSpin = false;
		}
		m_pLandingParticle->Reset();
		BossAttack(fTimeDelta);
	}
}

void CBoss3Hand::BossAttack(const _float& fTimeDelta)
{
	m_fAttackCoolDown += fTimeDelta;

	// 회전하고 
	if (0.75f > m_fAttackCoolDown)
		m_pTransform->Rotation(ROT_Y, D3DXToRadian(735.f * fTimeDelta));

	// 내려 찍기 (4.f)
	else if(8.f >= m_pTransform->m_vInfo[INFO_POS].z)
	{
		m_pTransform->m_vInfo[INFO_POS].z += 1.f;

		if (7.f < m_pTransform->m_vInfo[INFO_POS].z)
		{
			StopSound(SOUND_EFFECT_ENEMY);
			PlaySound_Effect(L"3.wav", SOUND_EFFECT_ENEMY, 1.f);
			BoundingBox box;
			_vec3 vInfo = m_pTransform->m_vInfo[INFO_POS];
			box.Offset(vInfo);
			m_pLandingParticle->Set_Size(2.f);
			m_pLandingParticle->Set_Options(2.f, 20.f);
			m_pLandingParticle->Set_SizeLifeTime(1.f);
			m_pLandingParticle->Set_BoundingBox(box);
			m_pLandingParticle->Start_Particle();
			dynamic_cast<CStage1Camera*>(Engine::Get_GameObject(L"Layer_Environment", L"Camera"))->Start_Camera_Shake(0.7f, 100, SHAKE_ALL);
		}
	}

	else if (1.f < m_fAttackCoolDown)
	{
		m_pTransform->m_vInfo[INFO_POS].z = 9.f;
		m_bAttack = false;
		m_fAttackCoolDown = 0.f;
		m_fCoolDown = 0.f;
		m_bSpin = true;
	}
}

void CBoss3Hand::IdleMove(const _float & fTimeDelta)
{
	m_fIdleCycle += fTimeDelta;

	// 왼쪽으로 회전
	if (m_bIdleMove && !m_bIdleStop)
	{
		if (m_fIdleCycle > 0.075f)
		{
			m_fIdleAngle += 2.f;
			m_pTransform->Rotation(ROT_Y, D3DXToRadian(7.5f));
			m_fIdleCycle = 0.f;

			if (m_fIdleAngle >= 10.f)
			{
				m_fIdleAngle = 0.f;
				m_bIdleMove = false;
				m_bIdleStop = true;
			}
		}
	}

	// 오른쪽으로 회전
	else if (!m_bIdleMove && !m_bIdleStop)
	{
		if (m_fIdleCycle > 0.075f)
		{
			m_fIdleAngle += 2.f;
			m_pTransform->Rotation(ROT_Y, D3DXToRadian(-7.5f));
			m_fIdleCycle = 0.f;

			if (m_fIdleAngle >= 10.f)
			{
				m_fIdleAngle = 0.f;
				m_bIdleMove = true;
				m_bIdleStop = true;
			}
		}
	}

	// 잠깐 멈출 때
	else if (m_bIdleStop)
	{
		if (m_fIdleCycle > 0.2f)
		{
			m_fIdleCycle = 0.f;
			m_bIdleStop = false;
		}			
	}
}

CBoss3Hand * CBoss3Hand::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos, _int iIndex)
{
	CBoss3Hand* pInstance = new CBoss3Hand(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, iIndex)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss3Hand::Free(void)
{
	__super::Free();
}
