#include "stdafx.h"
#include"Boss3.h"
#include "Boss3Hand.h"
#include"..\Client\Toodee.h"
#include"..\Client\Topdee.h"
#include"StageCamera.h"
#include "Export_Function.h"


CBoss3Hand::CBoss3Hand(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCube(pGraphicDev),
	m_fSpeed(27.f), m_fCoolDown(0.f), m_fAttackCoolDown(0.f), m_fIdleCycle(0.f), m_fIdleAngle(0.f),
	m_iIndex(0),
	m_bAttack(false), m_bIdleMove(true), m_bIdleStop(false)
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
	m_vPrePos = vPos;
	m_pCollider->Set_BoundingBox({ 4.f, 4.f, 4.f });
	m_pCollider->Set_Group(COL_OBJ);

	m_iIndex = iIndex;

	return S_OK;
}

_int CBoss3Hand::Update_GameObject(const _float & fTimeDelta)
{
	
	if (m_bDead)
		return OBJ_DEAD;
    
	if (m_bShock)
	{
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

	if (0.f > m_fAngle)
		m_pTransform->Rotation(ROT_X, D3DXToRadian(-(m_fAngle)++ * fTimeDelta));

	CGameObject::Update_Too(fTimeDelta);

	return 0;
}

_int CBoss3Hand::Update_Top(const _float & fTimeDelta)
{
	m_pTransform->Set_Pos(m_vPrePos.x, m_vPrePos.y, m_vPrePos.z);

	if(!m_pTransform->m_vInfo[INFO_POS].z == 3.f)
	{
		m_pTransform->m_vInfo[INFO_POS].z -= 1.f;
	}

	if (-100.f < m_fAngle)
		m_pTransform->Rotation(ROT_X, D3DXToRadian(m_fAngle-- * fTimeDelta));

	if (m_bAttack)
		FollowPlayer(fTimeDelta);
	m_vPrePos = m_pTransform->m_vInfo[INFO_POS];

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
	if(!g_Is2D)
	m_pShadowCom->Render_Shadow(m_pBufferCom, 0.75f, 0.75f, 1.f);

	if (m_bShock == true)
		m_pTextureCom->Set_Texture();
	else
		m_pTextureCom2->Set_Texture();
	m_pBufferCom->Render_Buffer();
	CGameObject::Render_GameObject();
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
		dynamic_cast<CTopdee*>(pGameObject)->Set_AnimDead();
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

void CBoss3Hand::SwapTrigger()
{
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

	return S_OK;
}

void CBoss3Hand::FollowPlayer(const _float & fTimeDelta)
{
	m_fCoolDown += fTimeDelta;


	CGameObject* pGameObject = Engine::Get_GameObject(L"Layer_GameLogic", L"Topdee");
	NULL_CHECK_RETURN(pGameObject, );

	// 추격을 진행하고
	if (1.5f < m_fCoolDown && 2.f + BOSS3_CHASE > m_fCoolDown)
	{
		m_pTransform->Chase_Target(&pGameObject->m_pTransform->m_vInfo[INFO_POS], m_fSpeed, fTimeDelta);

		if(m_pTransform->m_vInfo[INFO_POS].z <= 8.f);
			m_pTransform->m_vInfo[INFO_POS].z -= 31.f*fTimeDelta;
	}

	else if (4.f < m_fCoolDown )
	{
		BossAttack(fTimeDelta);
	}
}

void CBoss3Hand::BossAttack(const _float & fTimeDelta)
{
	m_fAttackCoolDown += fTimeDelta;

	// 회전하고 
	if (0.75f > m_fAttackCoolDown)
		m_pTransform->Rotation(ROT_Y, D3DXToRadian(735.f * fTimeDelta));

	// 내려 찍기 (4.f)
	else if(8.f >= m_pTransform->m_vInfo[INFO_POS].z)
	{
			m_pTransform->m_vInfo[INFO_POS].z += 1.f;
			int i = 0;

			if(7.f < m_pTransform->m_vInfo[INFO_POS].z)
				dynamic_cast<CStage1Camera*>(Engine::Get_GameObject(L"Layer_Environment", L"Camera"))->Start_Camera_Shake(0.7f, 100.0f, SHAKE_ALL);

	}

	else if (1.f < m_fAttackCoolDown)
	{
		m_pTransform->m_vInfo[INFO_POS].z = 9.f;
		m_bAttack = false;
		m_fAttackCoolDown = 0.f;
		m_fCoolDown = 0.f;
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
