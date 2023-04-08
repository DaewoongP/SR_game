#include "stdafx.h"
#include "Bat.h"
#include "Cube.h"
#include "Export_Function.h"

CBat::CBat(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev), m_bMoveLeft(false), m_bBackSprite(false), m_fZRatio(0.0f)
{
}

CBat::~CBat()
{
}

HRESULT CBat::Ready_GameObject(_vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fSpeed = 10.0f;
	m_pTransform->m_vScale = { -BATSCALE, BATSCALE, 1.f };
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_bIsStatic = false;
	m_pRigid->m_bUseGrivaty = false;
	m_pRigid->m_bUseLimitVelocity = true;
	m_pRigid->m_fLimitVelocity = 10.0f;
	m_pRigid->m_bFreezePos_Z = true;

	return S_OK;
}

_int CBat::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	m_pTextureCom->Update_Anim(fTimeDelta);
	m_pTextureCom_Back->Update_Anim(fTimeDelta);
	__super::Update_GameObject(fTimeDelta);
	return 0;
}

_int CBat::Update_Too(const _float & fTimeDelta)
{
	m_pTransform->m_vAngle.z = 0.0f;
	m_pTransform->m_vInfo[INFO_POS].z = 10.f;
	m_bBackSprite = false;

	m_pRigid->m_Velocity = { 0.0f, 0.0f, 0.0f };

	_vec3 MoveDir = { 1.0f,0.0f,0.0f };

	if (m_pTransform->m_vScale.y > BATSCALE)
	{
		m_pTransform->m_vScale.y -= BATSCALE * 0.1f;
	}
	else
		m_pTransform->m_vScale.y = BATSCALE;

	if (m_bMoveLeft)
	{
		m_pTransform->m_vScale.x = BATSCALE;
	}
	else if (!m_bMoveLeft)
	{
		m_pTransform->m_vScale.x = -BATSCALE;
		MoveDir *= -1;
	}

	m_pTransform->Move_Pos(&MoveDir, fTimeDelta, m_fSpeed);

	return 0;
}

_int CBat::Update_Top(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	//=================================================
	if (1.0f >= m_fZRatio)
	{
		//�� ���
		m_fZRatio += fTimeDelta * 4.0f;
	}
	else
		m_fZRatio = 1.0f;

	//�� ���̱�
	m_pTransform->m_vInfo[INFO_POS].z = Lerp(BATTOOZ, BATTOPZ, m_fZRatio);
	//=================================================

	CTransform*	pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player02", L"Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransformCom, -1);

	_vec3	vPlayerPos;

	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	if (vPlayerPos.y > m_pTransform->Get_WorldMatrixPointer()->_42)
	{
		m_bBackSprite = true;
	}
	else
		m_bBackSprite = false;

	if (vPlayerPos.x < m_pTransform->Get_WorldMatrixPointer()->_41)
	{
		m_pTransform->m_vScale.x = -BATSCALE;
	}
	else
		m_pTransform->m_vScale.x = BATSCALE;

	vPlayerPos = vPlayerPos - m_pTransform->m_vInfo[INFO_POS];

	D3DXVec3Normalize(&vPlayerPos, &vPlayerPos);

	m_pRigid->AddForce(vPlayerPos, m_fSpeed * 10.0f, FORCE, fTimeDelta);

	return 0;
}

void CBat::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CBat::LateUpdate_Too()
{

}

void CBat::LateUpdate_Top()
{

}

void CBat::Render_GameObject(void)
{
	__super::Render_GameObject();
}

void CBat::Render_Too()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

void CBat::Render_Top()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	if (m_bBackSprite)
	{
		m_pTextureCom_Back->Set_Texture(0);
	}
	else
		m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

void CBat::OnCollisionEnter(const Collision * collision)
{
	if ((collision->_dir == DIR_LEFT || collision->_dir == DIR_RIGHT) && dynamic_cast<CCube*>(collision->otherObj))
	{
		if (m_bMoveLeft)
		{
			m_bMoveLeft = false;
			m_pTransform->m_vScale.x = -BATSCALE;
		}
		else if (!m_bMoveLeft)
		{
			m_bMoveLeft = true;
			m_pTransform->m_vScale.x = BATSCALE;
		}
	}
	if (g_Is2D)
		__super::OnCollisionEnter(collision);
}

void CBat::OnCollisionStay(const Collision * collision)
{
	if (g_Is2D)
		__super::OnCollisionStay(collision);
}

HRESULT CBat::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"RcTex", pComponent });

	pComponent = m_pTextureCom_Back = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Bat_Back_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom_Back, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Bat_Back_Texture", pComponent });
	m_pTextureCom_Back->Add_Anim(L"Idle", 0, 6, 0.6f, true);
	m_pTextureCom_Back->Switch_Anim(L"Idle");
	m_pTextureCom_Back->m_bUseFrameAnimation = true;

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Bat_Front_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Bat_Front_Texture", pComponent });
	m_pTextureCom->Add_Anim(L"Idle", 0, 6, 0.6f, true);
	m_pTextureCom->Switch_Anim(L"Idle");
	m_pTextureCom->m_bUseFrameAnimation = true;

	pComponent = m_pRigid = dynamic_cast<CRigidbody*>(Engine::Clone_Proto(L"Rigidbody", this));
	NULL_CHECK_RETURN(m_pRigid, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Rigidbody", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Collider", pComponent });
	m_pCollider->Set_Options({2.f, 2.f, BATTOPZ}, COL_OBJ, true);
	m_pCollider->m_bIsTrigger = true;

	m_pTransform->m_bIsStatic = false;

	return S_OK;
}

CBat * CBat::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos)
{
	CBat*		pInstance = new CBat(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBat::Free(void)
{
	__super::Free();
}
