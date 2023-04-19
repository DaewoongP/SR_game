#include "stdafx.h"
#include "Boss3.h"
#include "Export_Function.h"
#include "StageCamera.h"
#include "AbstractFactory.h"

#include "Boss3Hand.h"
#include "Boss3HandPart.h"
#include "Boss3Eye.h"
#include "Boss3EyePupil.h"
#include "Boss3Eyebrow.h"
#include "Boss3Mouth.h"

CBoss3::CBoss3(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCube(pGraphicDev),
	m_fSpeed(25.f), m_fXAngle(0.f), m_fCoolDown(0.f), m_fAttackCoolDown(0.f), 
	m_fShootCoolDown(0.f), m_fPreToo(0.f), m_fPreTop(0.f), m_fShockDown(0.f),
	m_bCreateHand(true)
{
	m_pBossLeft = nullptr;
	m_pBossRight = nullptr;
}

CBoss3::~CBoss3()
{
}

HRESULT CBoss3::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = _vec3{ vPos.x, vPos.y, 7.f };
	m_pTransform->m_vScale = { 3.5f, 3.5f, 3.5f };
	m_pTransform->Rotation(ROT_Y, D3DXToRadian(-45.f));
	m_pTransform->m_bIsStatic = true;
	m_vPrePos = m_pTransform->m_vInfo[INFO_POS];
	m_pCollider->Set_BoundingBox({ 7.f, 7.f, 7.f });
	m_pCollider->Set_Group(COL_OBJ);

	m_pShadowCom->m_fShadowHeight = 13.0f;

	return S_OK;
}

_int CBoss3::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	if (12.f < m_fAttackCoolDown)
		m_fShockDown += fTimeDelta;

	// 탑디일 때 스파크 타고 흐르는 애니메이션
	if (m_iBossHp == 1 && 1.f < m_fShockDown)
	{
		if (m_fShockDown > 1.f && m_fShockDown < 1.2f)
		{
			dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LPart"))->Set_SparkOn(true);
			dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RPart"))->Set_SparkOn(true);
		}

		else if (m_fShockDown > 1.2f && m_fShockDown < 1.4f)
		{
			dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LPart"))->Set_SparkOn(false);
			dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RPart"))->Set_SparkOn(false);

			dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LPart1"))->Set_SparkOn(true);
			dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RPart1"))->Set_SparkOn(true);
		}

		else if (m_fShockDown > 1.4f && m_fShockDown < 1.6f)
		{
			dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LPart1"))->Set_SparkOn(false);
			dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RPart1"))->Set_SparkOn(false);

			dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LPart2"))->Set_SparkOn(true);
			dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RPart2"))->Set_SparkOn(true);
		}

		else if (m_fShockDown > 1.6f && m_fShockDown < 1.8f)
		{
			dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LPart2"))->Set_SparkOn(false);
			dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RPart2"))->Set_SparkOn(false);

			dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LPart3"))->Set_SparkOn(true);
			dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RPart3"))->Set_SparkOn(true);
		}

		else if (m_fShockDown > 1.8f && m_fShockDown < 2.f)
		{
			dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LPart3"))->Set_SparkOn(false);
			dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RPart3"))->Set_SparkOn(false);

			dynamic_cast<CBoss3Hand*>(m_pBossLeft)->Set_Shock(true);
			dynamic_cast<CBoss3Hand*>(m_pBossRight)->Set_Shock(true);

			m_fShockDown = 0.f;
			m_bATKCnt = false;
		}
	}

	// Boss3 생성과 크기 조정
	if (m_bCreateHand)
	{
		_vec3 vPos = m_pTransform->m_vInfo[INFO_POS];
		CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
		NULL_CHECK_RETURN(pStageLayer, E_FAIL);

		FAILED_CHECK_RETURN(FACTORY<CBoss3Hand>::Create(L"Boss3Left", pStageLayer, _vec3{ vPos.x - 20.f, vPos.y, vPos.z }, 0), E_FAIL);
		FAILED_CHECK_RETURN(FACTORY<CBoss3Hand>::Create(L"Boss3Right", pStageLayer, _vec3{ vPos.x + 20.f, vPos.y, vPos.z }, 1), E_FAIL);

		FAILED_CHECK_RETURN(FACTORY<CBoss3Eye>::Create(L"Boss3LeftEye", pStageLayer, _vec3{ -4.2f, 1.f, +2.5f }, 0), E_FAIL);
		FAILED_CHECK_RETURN(FACTORY<CBoss3Eye>::Create(L"Boss3RightEye", pStageLayer, _vec3{ -4.2f, 1.f, -2.5f }, 0), E_FAIL);

		FAILED_CHECK_RETURN(FACTORY<CBoss3EyePupil>::Create(L"BossLeftPupil", pStageLayer, _vec3{-4.4f, 1.f, +2.5f}, 1), E_FAIL);
		FAILED_CHECK_RETURN(FACTORY<CBoss3EyePupil>::Create(L"BossRightPupil", pStageLayer, _vec3{ -4.4f, 1.f, -2.5f }, 1), E_FAIL);

		FAILED_CHECK_RETURN(FACTORY<CBoss3Eyebrow>::Create(L"BossLeftEyebrow", pStageLayer, _vec3{ -4.2f, 4.f, +2.5f }, 2), E_FAIL);
		FAILED_CHECK_RETURN(FACTORY<CBoss3Eyebrow>::Create(L"BossRightEyebrow", pStageLayer, _vec3{ -4.2f, 4.f, -2.5f }, 2), E_FAIL);

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

		FAILED_CHECK_RETURN(FACTORY<CBoss3HandPart>::Create(L"Boss3LPart3Shadow", pStageLayer, _vec3{ vPos.x ,vPos.y,vPos.z },2 ), E_FAIL);
		FAILED_CHECK_RETURN(FACTORY<CBoss3HandPart>::Create(L"Boss3RPart3Shadow", pStageLayer, _vec3{ vPos.x ,vPos.y,vPos.z },2 ), E_FAIL);

		m_pBossLeft = Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3Left");
		m_pBossRight = Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3Right");
		MakeChain();

		m_pBossLeftEye = Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LeftEye");
		m_pBossLeftEye->m_pTransform->m_vScale = { 1.8f, 1.8f, 1.f };
		m_pBossRightEye = Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RightEye");
		m_pBossRightEye->m_pTransform->m_vScale = { 1.8f, 1.8f, 1.f };

		m_pBossLeftPupil = Engine::Get_GameObject(L"Layer_GameLogic", L"BossLeftPupil");
		m_pBossLeftPupil->m_pTransform->m_vScale = { 0.8f, 0.8f, 1.f };
		m_pBossRightPupil = Engine::Get_GameObject(L"Layer_GameLogic", L"BossRightPupil");
		m_pBossRightPupil->m_pTransform->m_vScale = { 0.8f, 0.8f, 1.f };

		m_pLeftEyebrow = Engine::Get_GameObject(L"Layer_GameLogic", L"BossLeftEyebrow");
		m_pLeftEyebrow->m_pTransform->m_vScale = { -1.2f, 1.2f, 1.f };
		m_pRightEyebrow = Engine::Get_GameObject(L"Layer_GameLogic", L"BossRightEyebrow");
		m_pRightEyebrow->m_pTransform->m_vScale = { 1.2f, 1.2f, 1.f };

		m_pMouth = Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3Mouth");
		m_pMouth->m_pTransform->m_vScale = { 8.f, 8.f, 1.f };

		m_pBossLeft = Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3Left");
		m_pBossRight = Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3Right");

		m_bCreateHand = false;
	}

	LookAtPlayer();

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

_int CBoss3::Update_Too(const _float & fTimeDelta)
{
	m_pTransform->m_vInfo[INFO_POS].z = 7.f;

	if (m_iATKCount == 3)
	{
		m_bShoot = false;
		m_fShootterm += fTimeDelta;

		// 스파크 이동 후 스파크 공격
		if (m_fShootterm > 1.f && m_iBossHp==1)
		{
			if (m_fShootterm > 1.f && m_fShootterm < 1.2f)
			{
				dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LPart"))->Set_SparkOn(true);
				dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RPart"))->Set_SparkOn(true);
			}

			else if (m_fShootterm > 1.2f && m_fShootterm < 1.4f)
			{
				dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LPart"))->Set_SparkOn(false);
				dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RPart"))->Set_SparkOn(false);

				dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LPart1"))->Set_SparkOn(true);
				dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RPart1"))->Set_SparkOn(true);
			}

			else if (m_fShootterm > 1.4f && m_fShootterm < 1.6f)
			{
				dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LPart1"))->Set_SparkOn(false);
				dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RPart1"))->Set_SparkOn(false);

				dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LPart2"))->Set_SparkOn(true);
				dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RPart2"))->Set_SparkOn(true);
			}

			else if (m_fShootterm > 1.6f && m_fShootterm < 1.8f)
			{
				dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LPart2"))->Set_SparkOn(false);
				dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RPart2"))->Set_SparkOn(false);

				dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LPart3"))->Set_SparkOn(true);
				dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RPart3"))->Set_SparkOn(true);
			}

			else if (m_fShootterm > 1.8f && m_fShootterm < 2.f)
			{
				dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LPart3"))->Set_SparkOn(false);
				dynamic_cast<CBoss3HandPart*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RPart3"))->Set_SparkOn(false);

				dynamic_cast<CBoss3Hand*>(m_pBossLeft)->Set_Shock(true);
				dynamic_cast<CBoss3Hand*>(m_pBossRight)->Set_Shock(true);
			}
		}
	}

	if (m_bShoot==false && m_fShootterm > 4.9f)//5이상 주게되면 전기패턴 루프돌아버림
	{
		StopSound(SOUND_EFFECT_ENEMY);
		m_bShoot = true;
		m_fShootterm = 0.f;
		m_iATKCount = 0;
		m_bSpin = true;

	}

	if (0.f > m_fXAngle)
		m_pTransform->Rotation(ROT_X, D3DXToRadian(-(m_fXAngle)++ * fTimeDelta));

	ShootBullet(fTimeDelta);

	CGameObject::Update_Too(fTimeDelta);

	return 0;
}

_int CBoss3::Update_Top(const _float& fTimeDelta)
{
	m_pTransform->Set_Pos(m_vPrePos.x, m_vPrePos.y, m_vPrePos.z);
	if (-100.f < m_fXAngle)
		m_pTransform->Rotation(ROT_X, D3DXToRadian(m_fXAngle-- * fTimeDelta));
	FollowPlayer(fTimeDelta);
	m_vPrePos = m_pTransform->m_vInfo[INFO_POS];

	CGameObject::Update_Top(fTimeDelta);

	return 0;
}

void CBoss3::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CBoss3::LateUpdate_Top()
{

}

void CBoss3::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	if(!g_Is2D)
		m_pShadowCom->Render_Shadow(m_pBufferCom);

	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();

	__super::Render_GameObject();
	m_pLandingParticle->Update_Particle();
}

void CBoss3::OnCollisionEnter(const Collision * collision)
{
	//collision->otherObj
	__super::OnCollisionEnter(collision);
}

void CBoss3::OnCollisionStay(const Collision * collision)
{
	__super::OnCollisionStay(collision);
}

void CBoss3::OnCollisionExit(const Collision * collision)
{
	__super::OnCollisionExit(collision);
}

void CBoss3::SwapTrigger()
{
	// 플레이어 전환을 할 때마다 보스의 시점을 초기화해주는 부분
	if(g_Is2D)
		m_pTransform->m_vAngle.y = D3DXToRadian(-45.f);

	else
		m_pTransform->m_vAngle.y = D3DXToRadian(-75.f);

	m_fPreToo = 0.f;
	m_fPreTop = 0.f;
}

void CBoss3::MakeChain()
{
	m_pBossLeftPart = Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LPart");
	m_pBossRightPart = Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RPart");
	m_pBossLeftPart1 = Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LPart1");
	m_pBossRightPart1 = Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RPart1");
	m_pBossLeftPart = Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LPart2");
	m_pBossRightPart = Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RPart2");
	m_pBossLeftPart1 = Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3LPart3");
	m_pBossRightPart1 = Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3RPart3");
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

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });

	pComponent = m_pShadowCom = dynamic_cast<CShadow*>(Engine::Clone_Proto(L"Shadow", this));
	NULL_CHECK_RETURN(m_pShadowCom, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Shadow",pComponent });

	pComponent = m_pLandingParticle = dynamic_cast<CCircularParticle*>(Engine::Clone_Proto(L"Boss2LandParticle", this));
	NULL_CHECK_RETURN(m_pLandingParticle, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"BossLandingParticle",pComponent });

	return S_OK;
}

void CBoss3::FollowPlayer(const _float & fTimeDelta)
{
	m_fCoolDown += fTimeDelta;

	CGameObject* pGameObject = Engine::Get_GameObject(L"Layer_GameLogic", L"Topdee");
	NULL_CHECK_RETURN(pGameObject, );
	
	// 추격을 진행하고
	if (2.f < m_fCoolDown && 2.f + BOSS3_CHASE > m_fCoolDown)
	{
		m_pTransform->Chase_Target(&pGameObject->m_pTransform->m_vInfo[INFO_POS], m_fSpeed, fTimeDelta);
		
		if (!(m_pTransform->m_vInfo[INFO_POS].z == 9.f))
			m_pTransform->m_vInfo[INFO_POS].z -= 0.5f;
		
	}
	
	// 시간이 지나면 공격 시작 (3.5f)
	else if (2.f + BOSS3_CHASE < m_fCoolDown)
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
	_vec3 vPos;
	_float fX;

	CGameObject* pToodee = nullptr;
	CGameObject* pTopdee = nullptr;

	// 투디
	if (g_Is2D)
	{
		pToodee = Engine::Get_GameObject(L"Layer_GameLogic", L"Toodee");
		NULL_CHECK_RETURN(pToodee, );
		vPos = pToodee->m_pTransform->m_vInfo[INFO_POS];

		// 투디 x값 정규화
		fX = (vPos.x - CUBEX) / CUBEX;

		// 이전에 회전한 수치만큼 원복했다가 새로운 수치만큼 다시 돌림
		m_pTransform->Rotation(ROT_Y, m_fPreToo * 0.7f);
		m_pTransform->Rotation(ROT_Y, -fX * 0.7f);

		// 현재 값을 저장
		m_fPreToo = fX;
	}

	// 탑디
	else
	{
		pTopdee = Engine::Get_GameObject(L"Layer_GameLogic", L"Topdee");
		NULL_CHECK_RETURN(pTopdee, );
		vPos = pTopdee->m_pTransform->m_vInfo[INFO_POS];

		_vec3 vRight, vDir;

		D3DXVec3Normalize(&vRight, &m_pTransform->m_vInfo[INFO_RIGHT]);
		D3DXVec3Normalize(&vDir, &(vPos - m_pTransform->m_vInfo[INFO_POS]));

		_float fAngle = acosf(D3DXVec3Dot(&vRight, &vDir));

		// 플레이어의 y값이 보스 몸체의 y값보다 작아질 때 (치역 처리)
		if (vPos.y < m_pTransform->m_vInfo[INFO_POS].y)
			fAngle =  (2.f * D3DX_PI - fAngle) - 0.47f;

		m_pTransform->Rotation(ROT_Y, m_fPreTop * 0.6f);
		m_pTransform->Rotation(ROT_Y, -fAngle * 0.6f);

		m_fPreTop = fAngle;
	}
}

void CBoss3::BossAttack(const _float & fTimeDelta)
{
	m_fAttackCoolDown += fTimeDelta;
	
	// 회전하고 
	if (0.75f > m_fAttackCoolDown)
	{
		
		m_pTransform->Rotation(ROT_Y, D3DXToRadian(735.f * fTimeDelta));
	}
	
	// 내려 찍기
	else if (0.75f < m_fAttackCoolDown && 1.f > m_fAttackCoolDown)
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
			dynamic_cast<CStage1Camera*>(Engine::Get_GameObject(L"Layer_Environment", L"Camera"))->Start_Camera_Shake(0.7f, 100.0f, SHAKE_ALL);

		}
	}

	// 왼손 공격 명령
	else if (1.f < m_fAttackCoolDown && 5.f > m_fAttackCoolDown)
	{
		dynamic_cast<CBoss3Hand*>(m_pBossLeft)->Set_Attack(true);
		dynamic_cast<CBoss3Hand*>(m_pBossRight)->Set_Attack(false);
		
		m_bATKEnd = true;
	}

	// 오른손 공격 명령
	else if (5.f < m_fAttackCoolDown && 8.f > m_fAttackCoolDown&&m_bATKEnd==true)
	{
		dynamic_cast<CBoss3Hand*>(m_pBossRight)->Set_Attack(true);
		if (m_iBossHp == 1)
			m_bATKCnt = true;
	}		
	
	else if (14.f < m_fAttackCoolDown&&m_bATKCnt==false)
	{
		m_fAttackCoolDown = 0.f;
		m_fCoolDown = -1.5f;	
		m_bSpin = true;
	}
	
}

void CBoss3::ShootBullet(const _float & fTimeDelta)
{
	m_fShootCoolDown += fTimeDelta;
	if (1.f < m_fShootCoolDown&&m_bShoot == true)
	{
		dynamic_cast<CBoss3Mouth*>(m_pMouth)->Set_ShootBullet();
		dynamic_cast<CBoss3Mouth*>(m_pMouth)->Set_Animation();

		++m_iATKCount;
		m_fShootCoolDown = 0.f;
	}	
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