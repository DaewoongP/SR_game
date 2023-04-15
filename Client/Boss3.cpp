#include "stdafx.h"
#include "Boss3.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

#include "Boss3Hand.h"
#include "Boss3Eye.h"
#include "Boss3EyePupil.h"
#include "Boss3Eyebrow.h"
#include "Boss3Mouth.h"
#include "Fireball.h"

CBoss3::CBoss3(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCube(pGraphicDev),
	m_fSpeed(25.f), m_fXAngle(0.f), m_fCoolDown(0.f), m_fAttackCoolDown(0.f), m_fShootCoolDown(0.f),
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
	
	m_pCollider->Set_BoundingBox({ 7.f, 7.f, 7.f });
	m_pCollider->Set_Group(COL_OBJ);

	return S_OK;
}

_int CBoss3::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	

	// Boss3 생성과 크기 조정
	if (m_bCreateHand)
	{
		_vec3 vPos = m_pTransform->m_vInfo[INFO_POS];
		CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
		NULL_CHECK_RETURN(pStageLayer, E_FAIL);

		FAILED_CHECK_RETURN(FACTORY<CBoss3Hand>::Create(L"Boss3Left", pStageLayer, _vec3{ vPos.x - 10.f, vPos.y, vPos.z }, 0), E_FAIL);
		FAILED_CHECK_RETURN(FACTORY<CBoss3Hand>::Create(L"Boss3Right", pStageLayer, _vec3{ vPos.x + 10.f, vPos.y, vPos.z }, 1), E_FAIL);

		FAILED_CHECK_RETURN(FACTORY<CBoss3Eye>::Create(L"Boss3LeftEye", pStageLayer, vPos, 0), E_FAIL);
		FAILED_CHECK_RETURN(FACTORY<CBoss3Eye>::Create(L"Boss3RightEye", pStageLayer, vPos, 0), E_FAIL);

		FAILED_CHECK_RETURN(FACTORY<CBoss3EyePupil>::Create(L"BossLeftPupil", pStageLayer, vPos, 1), E_FAIL);
		FAILED_CHECK_RETURN(FACTORY<CBoss3EyePupil>::Create(L"BossRightPupil", pStageLayer, vPos, 1), E_FAIL);

		FAILED_CHECK_RETURN(FACTORY<CBoss3Eyebrow>::Create(L"BossLeftEyebrow", pStageLayer, vPos, 2), E_FAIL);
		FAILED_CHECK_RETURN(FACTORY<CBoss3Eyebrow>::Create(L"BossRightEyebrow", pStageLayer, vPos, 2), E_FAIL);

		FAILED_CHECK_RETURN(FACTORY<CBoss3Mouth>::Create(L"Boss3Mouth", pStageLayer, vPos), E_FAIL);

		m_pBossLeft = Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3Left");
		m_pBossRight = Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3Right");

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

		m_bCreateHand = false;
	}

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

_int CBoss3::Update_Too(const _float & fTimeDelta)
{
	m_pTransform->m_vInfo[INFO_POS].z = 7.f;

	m_vLookDot = { m_pTransform->m_vInfo[INFO_POS].x - 1.f,
					   m_pTransform->m_vInfo[INFO_POS].y,
					   m_pTransform->m_vInfo[INFO_POS].z - 1.f };
	m_vDirection = m_vLookDot - m_pTransform->m_vInfo[INFO_POS];

	////toodee topdee 카메라전환할때 위치돌리기
	//if (0  >m_fXAngle)
	//	m_pTransform->Rotation(ROT_X, D3DXToRadian(-m_fXAngle++ * fTimeDelta));
	//
	//BossLook(fTimeDelta);
	//	m_vDirection = m_vLookDot - m_pTransform->m_vInfo[INFO_POS];

	//D3DXVec3Normalize(&m_vDirection, &m_vDirection);

	//_vec3 vBossToPlayer = m_vPlayerInfo - m_pTransform->m_vInfo[INFO_POS];
	//D3DXVec3Normalize(&vBossToPlayer, &vBossToPlayer);
	////_matrix matRot = *m_pTransform->Compute_Lookattarget(&m_vPlayerInfo);
	////D3DXMatrixRotationY(&matRot, D3DXToRadian(-45 * fTimeDelta));
	////m_pTransform->m_matWorld = matRot;
	////m_pTransform->m_matWorld = matRot;

	//_float fSight = D3DXVec3Dot(&vBossToPlayer, &m_vDirection);
	//if(!(fSight==0))
	//m_pTransform->Rotation(ROT_Y, D3DXToRadian(fSight));
	//if (fSight < 0.f)
	//{
	//	if (m_pTransform->m_vInfo[INFO_POS].y > m_vPlayerInfo.y)
	//	{
	//		if (m_pTransform->m_vInfo[INFO_POS].x < m_vPlayerInfo.x)
	//			m_pTransform->Rotation(ROT_Y, D3DXToRadian(-45 * fTimeDelta));
	//		if (m_pTransform->m_vInfo[INFO_POS].x > m_vPlayerInfo.x)
	//			m_pTransform->Rotation(ROT_Y, D3DXToRadian(45 * fTimeDelta));

	//	}
	//	else if (m_pTransform->m_vInfo[INFO_POS].y < m_vPlayerInfo.y)
	//	{
	//		if (m_pTransform->m_vInfo[INFO_POS].x < m_vPlayerInfo.x)
	//			m_pTransform->Rotation(ROT_Y, D3DXToRadian(-45 * fTimeDelta));
	//		if (m_pTransform->m_vInfo[INFO_POS].x > m_vPlayerInfo.x)
	//			m_pTransform->Rotation(ROT_Y, D3DXToRadian(45 * fTimeDelta));

	//	}
	//}
	//else if (fSight > 0.f)
	//{
	//	if (m_pTransform->m_vInfo[INFO_POS].y < m_vPlayerInfo.y)

	//		if (m_pTransform->m_vInfo[INFO_POS].x < m_vPlayerInfo.x)
	//			m_pTransform->Rotation(ROT_Y, D3DXToRadian(-45 * fTimeDelta));
	//	if (m_pTransform->m_vInfo[INFO_POS].x > m_vPlayerInfo.x)
	//		m_pTransform->Rotation(ROT_Y, D3DXToRadian(45 * fTimeDelta));
	//}
	//else if(fSight==0.f)
	//{	
	//	m_pTransform->Compute_Lookattarget(&m_vPlayerInfo);
	//}

	if (0.f > m_fXAngle)
		m_pTransform->Rotation(ROT_X, D3DXToRadian(-(m_fXAngle)++ * fTimeDelta));

	ShootBullet(fTimeDelta);

	CGameObject::Update_Too(fTimeDelta);

	return 0;
}

_int CBoss3::Update_Top(const _float & fTimeDelta)
{
	if (-100.f < m_fXAngle)
		m_pTransform->Rotation(ROT_X, D3DXToRadian(m_fXAngle-- * fTimeDelta));

	FollowPlayer(fTimeDelta);

	CGameObject::Update_Top(fTimeDelta);

	return 0;
}

void CBoss3::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CBoss3::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture();
	m_pBufferCom->Render_Buffer();
	__super::Render_GameObject();
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

	return S_OK;
}

void CBoss3::State_Change(const _float & fTimeDelta)
{
	if (m_ePreState != m_eCurState)
	{
		switch (m_eCurState)
		{
		case B3_IDLE:
			break;

		case B3_ATTACK:
			break;

		case B3_SHOOT:
			break;

		case B3_DEAD:
			break;
		}

		m_ePreState = m_eCurState;
	}	
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
		
		m_pTransform->m_vInfo[INFO_POS].z = -2.f;
	}

	// 시간이 지나면 공격 시작 (3.5f)
	else if (2.f + BOSS3_CHASE < m_fCoolDown)
		BossAttack(fTimeDelta);
}

void CBoss3::BossLook(const _float& fTimeDelta)
{
	CGameObject* pGameObject = Engine::Get_GameObject(L"Layer_GameLogic", L"Toodee");
	NULL_CHECK_RETURN(pGameObject, );
	m_vPlayerInfo = pGameObject->m_pTransform->m_vInfo[INFO_POS];
	
	m_pTransform->Compute_Lookattarget(&m_vPlayerInfo);

	
}

void CBoss3::BossAttack(const _float & fTimeDelta)
{
	m_fAttackCoolDown += fTimeDelta;

	// 회전하고 
	if (0.75f > m_fAttackCoolDown)
		m_pTransform->Rotation(ROT_Y, D3DXToRadian(735.f * fTimeDelta));

	// 내려 찍기
	else if (0.75f < m_fAttackCoolDown && 1.f > m_fAttackCoolDown)
	{
		if (8.f > m_pTransform->m_vInfo[INFO_POS].z)
			m_pTransform->m_vInfo[INFO_POS].z += 1.f; //* fTimeDelta; // 80.f 는 속도(상수)
		m_iATKCount++;

	}

	// 왼손 공격 명령
	else if (1.f < m_fAttackCoolDown && 5.f > m_fAttackCoolDown)
	{
		dynamic_cast<CBoss3Hand*>(m_pBossLeft)->Set_Attack(true);
		dynamic_cast<CBoss3Hand*>(m_pBossRight)->Set_Attack(false);
		m_iATKCount++;
		m_bATKEnd = true;
	}

	// 오른손 공격 명령
	else if (5.f < m_fAttackCoolDown && 8.f > m_fAttackCoolDown&&m_bATKEnd==true)
	{
		//CGameObject* pGameObject = Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3Right");
		//dynamic_cast<CBoss3Hand*>(m_pBossLeft)->Set_Attack(false);
		dynamic_cast<CBoss3Hand*>(m_pBossRight)->Set_Attack(true);
		m_iATKCount++;

	}		
	/*else if (m_iBossHp==1&& m_iATKCount == 3 && 8.f < m_fAttackCoolDown)
	{
		dynamic_cast<CBoss3Hand*>(m_pBossLeft)->Set_Shock(true);
		dynamic_cast<CBoss3Hand*>(m_pBossRight)->Set_Shock(true);
		m_fAttackCoolDown = 4.f;
		m_iATKCount = 0;
	}*/
	else if (8.f < m_fAttackCoolDown)
	{
		m_fAttackCoolDown = 0.f;
		m_fCoolDown = -1.5f;		
	}
}

void CBoss3::ShootBullet(const _float & fTimeDelta)
{
	m_fShootCoolDown += fTimeDelta;

	if (1.f < m_fShootCoolDown)
	{
		dynamic_cast<CBoss3Mouth*>(m_pMouth)->Set_Animation();

		_vec3 vPos = m_pTransform->m_vInfo[INFO_POS];
		CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
		NULL_CHECK_RETURN(pStageLayer,);

		FAILED_CHECK_RETURN(FACTORY<CFireball>::Create(L"Fireball", pStageLayer, vPos), );

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