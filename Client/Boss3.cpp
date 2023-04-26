#include "stdafx.h"
#include "Boss3.h"
#include "Export_Function.h"
#include "StageCamera.h"
#include "AbstractFactory.h"

#include "Toodee.h"
#include "Topdee.h"

#include "Boss3Hand.h"
#include "Boss3HandPart.h"
#include "Boss3Eye.h"
#include "Boss3EyePupil.h"
#include "Boss3Eyebrow.h"
#include "Boss3Mouth.h"

CBoss3::CBoss3(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCube(pGraphicDev),
	m_fTooTime(0.f), m_fTopTime(0.f), m_fSpeed(35.f), m_fFinalTime(0.f),
	m_fPreTop(0.f), m_fShockDown(0.f), m_fLerpDist(0.f), m_fDamagedTime(0.f), m_fDelay(0.f),
	m_bInit(true), m_bLerpMove(false), m_bDamaged(false), m_bDelay(true), m_bFinal(false)
{
	m_pToodee = nullptr;
	m_pTopdee = nullptr;
	m_pBoss3Mouth = nullptr;
	m_pBossLeftHand = nullptr;
	m_pBossRightHand = nullptr;

	m_pBoss3LPart = nullptr;
	m_pBoss3RPart = nullptr;
	m_pBoss3LPart1 = nullptr;
	m_pBoss3RPart1 = nullptr;
	m_pBoss3LPart2 = nullptr;
	m_pBoss3RPart2 = nullptr;
	m_pBoss3LPart3 = nullptr;
	m_pBoss3RPart3 = nullptr;
}

CBoss3::~CBoss3()
{
}

HRESULT CBoss3::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = _vec3{ vPos.x, vPos.y, 10.f };
	m_pTransform->m_vScale = { 3.5f, 3.5f, 3.5f };
	m_pTransform->m_vAngle.y = D3DXToRadian(-45.f);
	m_pTransform->m_bIsStatic = true;
	m_pCollider->Set_BoundingBox({ 7.f, 7.f, 7.f });
	m_pCollider->Set_Group(COL_OBJ);
	m_iHp = 3;
	m_pShadowCom->m_fShadowHeight = 13.0f;
	m_pShadowCom->m_bUseOutLine = false;

	return S_OK;
}

_int CBoss3::Update_Too(const _float & fTimeDelta)
{
	if (nullptr == m_pToodee || m_bDamaged || m_bDelay)
		return 0;

	m_pTransform->m_vInfo[INFO_POS].z = 7.f;

	if (m_iATKCount == 3)
	{
		m_bShoot = false;
		m_fShootterm += fTimeDelta;

		// 스파크 이동 후 스파크 공격
		if (m_fShootterm > 1.f && m_iHp == 1)
			Chain_Spark(m_fShootterm, fTimeDelta);
	}

	if (m_bShoot == false && m_fShootterm > 4.9f)//5이상 주게되면 전기패턴 루프돌아버림
	{
		StopSound(SOUND_EFFECT_ENEMY);
		m_bShoot = true;
		m_bSpin = true;
		m_fShootterm = 0.f;
		m_iATKCount = 0;
	}

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

	ShootBullet(fTimeDelta);

	CGameObject::Update_Too(fTimeDelta);

	return 0;
}

_int CBoss3::Update_Top(const _float& fTimeDelta)
{
	if (nullptr == m_pTopdee || m_bDamaged || m_bDelay)
		return 0;

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

	FollowPlayer(fTimeDelta);

	CGameObject::Update_Top(fTimeDelta);

	return 0;
}

_int CBoss3::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
	{
		Boss3PartDead();
		return OBJ_DEAD;
	}

	if (0 >= m_iHp)
	{
		m_bFinal = true;
		FinalDamaged(fTimeDelta);
	}

	if (m_bDelay)
		Delay(fTimeDelta);

	if (m_bDamaged)
	{
		DamagedBoss3(fTimeDelta);

		if (m_bLerpMove)
			Lerp_Moving(fTimeDelta);
	}

	// Boss3 생성과 크기 조정
	if (m_bInit)
		CreateParts();

	if (m_pTopdee->m_bDead || m_pToodee->m_bDead)
		return 0;

	LookAtPlayer();

	CGameObject::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CBoss3::LateUpdate_GameObject(void)
{		
	__super::LateUpdate_GameObject();
}

void CBoss3::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	if (m_bFinal)
	{
		if (0 == (_int)(m_fFinalTime * 250.f) % 2)
			m_pTextureCom->Set_Texture();

		else
			m_pTextureCom2->Set_Texture();
	}

	else
	{
		if (m_bDamaged)
		{
			if (0 == (_int)(m_fLerpDist * 250.f) % 2)
				m_pTextureCom->Set_Texture();

			else
				m_pTextureCom2->Set_Texture();
		}

		else
			m_pTextureCom->Set_Texture();

		if (!g_Is2D)
			m_pShadowCom->Render_Shadow(m_pBufferCom);
	}
	
	m_pBufferCom->Render_Buffer();

	CGameObject::Render_GameObject();

	m_pLandingParticle->Update_Particle();
	m_pScreamParticle->Update_Particle();
}

void CBoss3::OnCollisionEnter(const Collision * collision)
{
	if (collision->_dir == DIR_BACK)
	{
		TOPDEEDIE;
	}
	CGameObject::OnCollisionEnter(collision);
}

void CBoss3::OnCollisionStay(const Collision * collision)
{
	CGameObject::OnCollisionStay(collision);
}

void CBoss3::OnCollisionExit(const Collision * collision)
{
	CGameObject::OnCollisionExit(collision);
}

void CBoss3::SwapTrigger()
{
	// 플레이어 전환을 할 때마다 보스의 시점을 초기화해주는 부분
	if (g_Is2D)
		m_pTransform->m_vAngle.y = D3DXToRadian(-45.f);

	else
		m_pTransform->m_vAngle.y = D3DXToRadian(-75.f);

	m_fPreTop = 0.f;
}

HRESULT CBoss3::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"CubeTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"CubeTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Stage3_Boss_Cube", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Stage3_Boss_Cube", pComponent });

	pComponent = m_pTextureCom2 = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Stage3_Boss_Hand_Blank_Cube", this));
	NULL_CHECK_RETURN(m_pTextureCom2, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Stage3_Boss_Hand_Blank_Cube", pComponent });

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

void CBoss3::FollowPlayer(const _float & fTimeDelta)
{
	if (m_bLerpMove)
		return;

	m_fTopTime += fTimeDelta;

	// 추격을 진행하고
	if (2.f < m_fTopTime && 3.5f > m_fTopTime)
	{
		m_pTransform->Chase_Target(&m_pTopdee->m_pTransform->m_vInfo[INFO_POS], m_fSpeed, fTimeDelta);

		if (!(m_pTransform->m_vInfo[INFO_POS].z == 9.f))
			m_pTransform->m_vInfo[INFO_POS].z -= 0.5f;
	}

	// 시간이 지나면 공격 시작
	else if (3.5f < m_fTopTime)
	{
		if (m_bSpin)
		{
			StopSound(SOUND_EFFECT_ENEMY);
			PlaySound_Effect(L"79.wav", SOUND_EFFECT_ENEMY, 1.f);
			m_bSpin = false;
		}

		BossAttack(fTimeDelta);
	}
}

void CBoss3::LookAtPlayer()
{
	if (m_bLerpMove)
		return;

	_vec3 vPos;
	_float fX;

	// 투디
	if (g_Is2D)
	{
		vPos = m_pToodee->m_pTransform->m_vInfo[INFO_POS];

		// 투디 x값 정규화
		fX = (vPos.x - CUBEX) / CUBEX;

		m_pTransform->m_vAngle.y = -fX * 0.7f + 40.f;
	}

	// 탑디
	else
	{
		vPos = m_pTopdee->m_pTransform->m_vInfo[INFO_POS];

		_vec3 vRight, vDir;

		D3DXVec3Normalize(&vRight, &m_pTransform->m_vInfo[INFO_RIGHT]);
		D3DXVec3Normalize(&vDir, &(vPos - m_pTransform->m_vInfo[INFO_POS]));

		_float fAngle = acosf(D3DXVec3Dot(&vRight, &vDir));

		// 플레이어의 y값이 보스 몸체의 y값보다 작아질 때 (치역 처리)
		if (vPos.y < m_pTransform->m_vInfo[INFO_POS].y)
			fAngle = (2.f * D3DX_PI - fAngle) - 0.47f;

		m_pTransform->Rotation(ROT_Y, m_fPreTop * 0.6f);
		m_pTransform->Rotation(ROT_Y, -fAngle * 0.6f);

		m_fPreTop = fAngle;
	}
}

void CBoss3::BossAttack(const _float & fTimeDelta)
{
	// 회전하고 
	if (4.25f > m_fTopTime)
	{
		m_pTransform->Rotation(ROT_Y, D3DXToRadian(720.f * fTimeDelta));
	}

	// 내려 찍기
	else if (4.25f < m_fTopTime && 4.5f > m_fTopTime)
	{
		if (8.f > m_pTransform->m_vInfo[INFO_POS].z)
			m_pTransform->m_vInfo[INFO_POS].z += 80.f* fTimeDelta; // 80.f 는 속도(상수)

		else
		{
			StopSound(SOUND_EFFECT_ENEMY);
			PlaySound_Effect(L"3.wav", SOUND_EFFECT_ENEMY, 1.f);
			BoundingBox box;
			_vec3 vInfo = m_pTransform->m_vInfo[INFO_POS];
			box.Offset(vInfo);
			m_pLandingParticle->Set_Size(2.f);
			m_pLandingParticle->Set_Options(2.f, 25.f);
			m_pLandingParticle->Set_SizeLifeTime(1.f);
			m_pLandingParticle->Set_BoundingBox(box);
			m_pLandingParticle->Start_Particle();
			dynamic_cast<CStage1Camera*>(Engine::Get_GameObject(L"Layer_Environment", L"Camera"))->Start_Camera_Shake(0.7f, 100, SHAKE_ALL);
		}
	}

	// 왼손 공격 명령
	else if (4.5f < m_fTopTime && 8.5f > m_fTopTime)
		m_pBossLeftHand->Set_Attack(true);

	// 오른손 공격 명령
	else if (8.5f < m_fTopTime && 12.5f > m_fTopTime)
		m_pBossRightHand->Set_Attack(true);

	// 체력 여부에 따른 패턴 마무리(3, 2)
	else if (1 != m_iHp && 14.f < m_fTopTime)
	{
		m_fTopTime = 0.f;
		m_bSpin = true;
	}

	// 체력 여부에 따른 패턴 마무리(1)
	else if (1 == m_iHp && 14.f < m_fTopTime)
	{
		m_fShockDown += fTimeDelta;

		// 탑디일 때 스파크 타고 흐르는 애니메이션
		if (1.f < m_fShockDown)
		{
			Chain_Spark(m_fShockDown, fTimeDelta);

			if (2.f < m_fShockDown)
			{
				m_fShockDown = 0.f;
				m_fTopTime = 0.f;
			}
		}
	}
}

void CBoss3::ShootBullet(const _float & fTimeDelta)
{
	if (m_bLerpMove)
		return;

	m_fTooTime += fTimeDelta;

	if (1.f < m_fTooTime && m_bShoot)
	{
		m_pBoss3Mouth->Set_ShootBullet();
		m_pBoss3Mouth->Set_Animation();

		++m_iATKCount;
		m_fTooTime = 0.f;
	}
}

void CBoss3::Do_Scream(const _float & fTimeDelta)
{
	dynamic_cast<CStage1Camera*>(Engine::Get_GameObject(L"Layer_Environment", L"Camera"))->Start_Camera_Shake(4.f, 40, SHAKE_ALL);

	BoundingBox box;
	_vec3 vPos = m_pTransform->m_vInfo[INFO_POS];
	box.Offset(_vec3{ vPos.x, vPos.y, 0.f });
	m_pScreamParticle->Set_BoundingBox(box);
	m_pScreamParticle->Set_SizeLifeTime(1.07f);
	m_pScreamParticle->Start_Particle();
}

void CBoss3::End_Scream(const _float & fTimeDelta)
{
	m_pScreamParticle->End_Particle();
}

void CBoss3::Lerp_Moving(const _float & fTimeDelta)
{
	_vec3 vLerp;
	_vec3 vGoal = _vec3(30.f, 18.f, 10.f);

	D3DXVec3Lerp(&vLerp, &m_pTransform->m_vInfo[INFO_POS], &vGoal, m_fLerpDist);
	m_pTransform->m_vInfo[INFO_POS].x = vLerp.x;
	m_pTransform->m_vInfo[INFO_POS].y = vLerp.y;

	m_fLerpDist += fTimeDelta * 0.01f;
}

void CBoss3::Chain_Spark(_float fCoolDown, const _float& fTimeDelta)
{
	if (fCoolDown > 1.f && fCoolDown < 1.2f)
	{
		Do_Scream(fTimeDelta);

		m_pBoss3LPart->Set_SparkOn(true);
		m_pBoss3RPart->Set_SparkOn(true);
	}

	else if (fCoolDown > 1.2f && fCoolDown < 1.4f)
	{
		m_pBoss3LPart->Set_SparkOn(false);
		m_pBoss3RPart->Set_SparkOn(false);

		m_pBoss3LPart1->Set_SparkOn(true);
		m_pBoss3RPart1->Set_SparkOn(true);
	}

	else if (fCoolDown > 1.4f && fCoolDown < 1.6f)
	{
		m_pBoss3LPart1->Set_SparkOn(false);
		m_pBoss3RPart1->Set_SparkOn(false);

		m_pBoss3LPart2->Set_SparkOn(true);
		m_pBoss3RPart2->Set_SparkOn(true);
	}

	else if (fCoolDown > 1.6f && fCoolDown < 1.8f)
	{
		m_pBoss3LPart2->Set_SparkOn(false);
		m_pBoss3RPart2->Set_SparkOn(false);

		m_pBoss3LPart3->Set_SparkOn(true);
		m_pBoss3RPart3->Set_SparkOn(true);
	}

	else if (fCoolDown > 1.8f && fCoolDown < 2.f)
	{
		m_pBoss3LPart3->Set_SparkOn(false);
		m_pBoss3RPart3->Set_SparkOn(false);

		m_pBossLeftHand->Set_Shock(true);
		m_pBossRightHand->Set_Shock(true);

		End_Scream(fTimeDelta);
	}
}

void CBoss3::Delay(const _float & fTimeDelta)
{
	_float fFinishDelayTime = 12.f;

	m_fDelay += fTimeDelta;

	m_fShootterm = 0;
	m_fShockDown = 0;
	m_fLerpDist  = 0;
	m_fTooTime = 0;
	m_iATKCount  = 0;

	if (fFinishDelayTime <= m_fDelay)
	{
		Do_Scream(fTimeDelta);
		m_bDelay = false;
	}		
}

void CBoss3::Boss3PartDead()
{
	m_pBossLeftHand->m_bDead = true;
	m_pBossRightHand->m_bDead = true;

	Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LeftEye")->m_bDead = true;
	Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RightEye")->m_bDead = true;
	Engine::Get_GameObject(L"Layer_GameLogic", L"BossLeftPupil")->m_bDead = true;
	Engine::Get_GameObject(L"Layer_GameLogic", L"BossRightPupil")->m_bDead = true;
	Engine::Get_GameObject(L"Layer_GameLogic", L"BossLeftEyebrow")->m_bDead = true;
	Engine::Get_GameObject(L"Layer_GameLogic", L"BossRightEyebrow")->m_bDead = true;
	m_pBoss3Mouth->m_bDead = true;

	m_pBoss3LPart->m_bDead = true;
	m_pBoss3RPart->m_bDead = true;
	m_pBoss3LPart1->m_bDead = true;
	m_pBoss3RPart1->m_bDead = true;
	m_pBoss3LPart2->m_bDead = true;
	m_pBoss3RPart2->m_bDead = true;
	m_pBoss3LPart3->m_bDead = true;
	m_pBoss3RPart3->m_bDead = true;

	Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LPartShadow")->m_bDead = true;
	Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RPartShadow")->m_bDead = true;
	Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LPart1Shadow")->m_bDead = true;
	Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RPart1Shadow")->m_bDead = true;
	Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LPart2Shadow")->m_bDead = true;
	Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RPart2Shadow")->m_bDead = true;
	Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LPart3Shadow")->m_bDead = true;
	Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RPart3Shadow")->m_bDead = true;
}

void CBoss3::WhiteTwinkl()
{
	dynamic_cast<CBoss3Eye*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LeftEye"))->Set_Damaged();
	dynamic_cast<CBoss3Eye*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RightEye"))->Set_Damaged();

	dynamic_cast<CBoss3EyePupil*>(Engine::Get_GameObject(L"Layer_GameLogic", L"BossLeftPupil"))->Set_Damaged();
	dynamic_cast<CBoss3EyePupil*>(Engine::Get_GameObject(L"Layer_GameLogic", L"BossRightPupil"))->Set_Damaged();

	dynamic_cast<CBoss3Eyebrow*>(Engine::Get_GameObject(L"Layer_GameLogic", L"BossLeftEyebrow"))->Set_Damaged();
	dynamic_cast<CBoss3Eyebrow*>(Engine::Get_GameObject(L"Layer_GameLogic", L"BossRightEyebrow"))->Set_Damaged();

	Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LeftEye")->m_pTransform->m_vScale = { 3.f, 3.f, 1.f };
	Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RightEye")->m_pTransform->m_vScale = { 3.f, 3.f, 1.f };

	Engine::Get_GameObject(L"Layer_GameLogic", L"BossLeftPupil")->m_pTransform->m_vScale = { 1.5f, 1.5f, 1.f };
	Engine::Get_GameObject(L"Layer_GameLogic", L"BossRightPupil")->m_pTransform->m_vScale = { 1.5f, 1.5f, 1.f };

	Engine::Get_GameObject(L"Layer_GameLogic", L"BossLeftEyebrow")->m_pTransform->m_vScale = { -2.f, 2.f, 1.f };
	Engine::Get_GameObject(L"Layer_GameLogic", L"BossRightEyebrow")->m_pTransform->m_vScale = { 2.f, 2.f, 1.f };

	m_pBoss3Mouth->Set_Damaged();

	m_pBoss3LPart->Set_Damaged();
	m_pBoss3RPart->Set_Damaged();
	m_pBoss3LPart1->Set_Damaged();
	m_pBoss3RPart1->Set_Damaged();
	m_pBoss3LPart2->Set_Damaged();
	m_pBoss3RPart2->Set_Damaged();
	m_pBoss3LPart3->Set_Damaged();
	m_pBoss3RPart3->Set_Damaged();

	m_pBossLeftHand->Set_Damaged();
	m_pBossRightHand->Set_Damaged();
}

void CBoss3::FinalDamaged(const _float & fTimeDelta)
{
	m_fFinalTime += fTimeDelta * 0.01f;

	dynamic_cast<CStage1Camera*>(Engine::Get_GameObject(L"Layer_Environment", L"Camera"))->Start_Camera_Shake(3.f, 50, SHAKE_ALL);

	WhiteTwinkl();

	if (3.f <= m_fFinalTime * 100.f)
	{
		m_bDead = true;
	}
}

void CBoss3::DamagedBoss3(const _float& fTimeDelta)
{
	m_fDamagedTime += fTimeDelta;

	dynamic_cast<CStage1Camera*>(Engine::Get_GameObject(L"Layer_Environment", L"Camera"))->Start_Camera_Shake(3.f, 25, SHAKE_ALL);

	WhiteTwinkl();

	if (3.f <= m_fDamagedTime)
	{
		Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LeftEye")->m_pTransform->m_vScale = { 1.8f, 1.8f, 1.f };
		Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RightEye")->m_pTransform->m_vScale = { 1.8f, 1.8f, 1.f };

		Engine::Get_GameObject(L"Layer_GameLogic", L"BossLeftPupil")->m_pTransform->m_vScale = { 0.8f, 0.8f, 1.f };
		Engine::Get_GameObject(L"Layer_GameLogic", L"BossRightPupil")->m_pTransform->m_vScale = { 0.8f, 0.8f, 1.f };

		Engine::Get_GameObject(L"Layer_GameLogic", L"BossLeftEyebrow")->m_pTransform->m_vScale = { -1.2f, 1.2f, 1.f };
		Engine::Get_GameObject(L"Layer_GameLogic", L"BossRightEyebrow")->m_pTransform->m_vScale = { 1.2f, 1.2f, 1.f };

		m_bDamaged = false;
		m_fDamagedTime = 0.f;

		m_bLerpMove = false;
		m_fLerpDist = 0.f;

		m_fTooTime = 0.f;
		m_fTopTime = 0.f;

		m_fShootterm = 0.f;
		m_fShockDown = 0.f;
		m_fTimer	 = 0.f;
		m_fPreTop	 = 0.f;
	}

	return;
}

HRESULT CBoss3::CreateParts()
{
	_vec3 vPos = m_pTransform->m_vInfo[INFO_POS];
	CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
	NULL_CHECK_RETURN(pStageLayer, E_FAIL);

	FAILED_CHECK_RETURN(FACTORY<CBoss3Hand>::Create(L"Boss3Left", pStageLayer, _vec3{ vPos.x - 15.f, vPos.y, vPos.z }, 0), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CBoss3Hand>::Create(L"Boss3Right", pStageLayer, _vec3{ vPos.x + 15.f, vPos.y, vPos.z }, 1), E_FAIL);

	FAILED_CHECK_RETURN(FACTORY<CBoss3Eye>::Create(L"Boss3LeftEye", pStageLayer, _vec3{ -4.2f, 1.f, +2.5f }, 0), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CBoss3Eye>::Create(L"Boss3RightEye", pStageLayer, _vec3{ -4.2f, 1.f, -2.5f }, 0), E_FAIL);

	FAILED_CHECK_RETURN(FACTORY<CBoss3EyePupil>::Create(L"BossLeftPupil", pStageLayer, _vec3{ -4.4f, 1.f, +2.5f }, 1), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CBoss3EyePupil>::Create(L"BossRightPupil", pStageLayer, _vec3{ -4.4f, 1.f, -2.5f }, 1), E_FAIL);

	FAILED_CHECK_RETURN(FACTORY<CBoss3Eyebrow>::Create(L"BossLeftEyebrow", pStageLayer, _vec3{ -4.21f, 4.f, +2.5f }, 2), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CBoss3Eyebrow>::Create(L"BossRightEyebrow", pStageLayer, _vec3{ -4.21f, 4.f, -2.5f }, 2), E_FAIL);

	FAILED_CHECK_RETURN(FACTORY<CBoss3Mouth>::Create(L"Boss3Mouth", pStageLayer, _vec3{ -5.f, -1.f, 0.f }), E_FAIL);

	FAILED_CHECK_RETURN(FACTORY<CBoss3HandPart>::Create(L"Boss3LPart", pStageLayer, _vec3{ vPos.x - 2.f ,vPos.y,vPos.z }, 2), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CBoss3HandPart>::Create(L"Boss3RPart", pStageLayer, _vec3{ vPos.x + 2.f ,vPos.y,vPos.z }, 2), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CBoss3HandPart>::Create(L"Boss3LPartShadow", pStageLayer, _vec3{ vPos.x  ,vPos.y,vPos.z }, 0), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CBoss3HandPart>::Create(L"Boss3RPartShadow", pStageLayer, _vec3{ vPos.x  ,vPos.y,vPos.z }, 0), E_FAIL);

	FAILED_CHECK_RETURN(FACTORY<CBoss3HandPart>::Create(L"Boss3LPart1", pStageLayer, _vec3{ vPos.x - 5.f,vPos.y,vPos.z }, 4), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CBoss3HandPart>::Create(L"Boss3RPart1", pStageLayer, _vec3{ vPos.x + 5.f,vPos.y,vPos.z }, 4), E_FAIL);

	FAILED_CHECK_RETURN(FACTORY<CBoss3HandPart>::Create(L"Boss3LPart1Shadow", pStageLayer, _vec3{ vPos.x  ,vPos.y ,vPos.z }, 1), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CBoss3HandPart>::Create(L"Boss3RPart1Shadow", pStageLayer, _vec3{ vPos.x  ,vPos.y ,vPos.z }, 1), E_FAIL);

	FAILED_CHECK_RETURN(FACTORY<CBoss3HandPart>::Create(L"Boss3LPart2", pStageLayer, _vec3{ vPos.x - 8.f,vPos.y,vPos.z }, 2), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CBoss3HandPart>::Create(L"Boss3RPart2", pStageLayer, _vec3{ vPos.x + 8.f,vPos.y,vPos.z }, 2), E_FAIL);

	FAILED_CHECK_RETURN(FACTORY<CBoss3HandPart>::Create(L"Boss3LPart2Shadow", pStageLayer, _vec3{ vPos.x ,vPos.y,vPos.z }, 0), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CBoss3HandPart>::Create(L"Boss3RPart2Shadow", pStageLayer, _vec3{ vPos.x ,vPos.y,vPos.z }, 0), E_FAIL);

	FAILED_CHECK_RETURN(FACTORY<CBoss3HandPart>::Create(L"Boss3LPart3", pStageLayer, _vec3{ vPos.x - 10.f,vPos.y,vPos.z }, 0), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CBoss3HandPart>::Create(L"Boss3RPart3", pStageLayer, _vec3{ vPos.x + 10.f,vPos.y,vPos.z }, 0), E_FAIL);

	FAILED_CHECK_RETURN(FACTORY<CBoss3HandPart>::Create(L"Boss3LPart3Shadow", pStageLayer, _vec3{ vPos.x ,vPos.y,vPos.z }, 2), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CBoss3HandPart>::Create(L"Boss3RPart3Shadow", pStageLayer, _vec3{ vPos.x ,vPos.y,vPos.z }, 2), E_FAIL);

	m_pToodee = dynamic_cast<CToodee*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Toodee"));
	m_pTopdee = dynamic_cast<CTopdee*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Topdee"));
	m_pBoss3Mouth = dynamic_cast<CBoss3Mouth*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3Mouth"));
	m_pBossLeftHand = dynamic_cast<CBoss3Hand*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3Left"));
	m_pBossRightHand = dynamic_cast<CBoss3Hand*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3Right"));

	m_pBoss3LPart = dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LPart"));;
	m_pBoss3RPart = dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RPart"));;
	m_pBoss3LPart1 = dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LPart1"));;
	m_pBoss3RPart1 = dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RPart1"));;
	m_pBoss3LPart2 = dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LPart2"));;
	m_pBoss3RPart2 = dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RPart2"));;
	m_pBoss3LPart3 = dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LPart3"));;
	m_pBoss3RPart3 = dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RPart3"));;

	Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LeftEye")->m_pTransform->m_vScale = { 1.8f, 1.8f, 1.f };
	Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RightEye")->m_pTransform->m_vScale = { 1.8f, 1.8f, 1.f };

	Engine::Get_GameObject(L"Layer_GameLogic", L"BossLeftPupil")->m_pTransform->m_vScale = { 0.8f, 0.8f, 1.f };
	Engine::Get_GameObject(L"Layer_GameLogic", L"BossRightPupil")->m_pTransform->m_vScale = { 0.8f, 0.8f, 1.f };

	Engine::Get_GameObject(L"Layer_GameLogic", L"BossLeftEyebrow")->m_pTransform->m_vScale = { -1.2f, 1.2f, 1.f };
	Engine::Get_GameObject(L"Layer_GameLogic", L"BossRightEyebrow")->m_pTransform->m_vScale = { 1.2f, 1.2f, 1.f };

	m_pBoss3Mouth->m_pTransform->m_vScale = { 8.f, 8.f, 1.f };

	m_bInit = false;

	return S_OK;
}

CBoss3 * CBoss3::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CBoss3* pInstance = new CBoss3(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss3::Free(void)
{
	__super::Free();
}