#include "stdafx.h"
#include "Boss3.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

#include "Boss3Hand.h"

CBoss3::CBoss3(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCube(pGraphicDev),
	m_fSpeed(25.f),
	m_bCreateHand(true)
{
}

CBoss3::~CBoss3()
{
}

HRESULT CBoss3::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vScale = { 3.5f, 3.5f, 3.5f };
	m_pTransform->m_bIsStatic = true;

	m_pCollider->Set_BoundingBox({ 7.f, 7.f, 7.f });
	m_pCollider->Set_Group(COL_OBJ);

	return S_OK;
}

_int CBoss3::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_bCreateHand)
	{
		_vec3 vPos = m_pTransform->m_vInfo[INFO_POS];
		CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
		NULL_CHECK_RETURN(pStageLayer, E_FAIL);

		FAILED_CHECK_RETURN(FACTORY<CBoss3Hand>::Create(L"Boss3Left", pStageLayer, _vec3{ vPos.x - 10.f, vPos.y, vPos.z }, 0), E_FAIL);
		FAILED_CHECK_RETURN(FACTORY<CBoss3Hand>::Create(L"Boss3Right", pStageLayer, _vec3{ vPos.x + 10.f, vPos.y, vPos.z }, 1), E_FAIL);

		m_bCreateHand = false;
	}

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

_int CBoss3::Update_Too(const _float & fTimeDelta)
{
	if (0.f > m_fAngle)
		m_pTransform->Rotation(ROT_X, D3DXToRadian(-(m_fAngle)++ * fTimeDelta));

	CGameObject::Update_Too(fTimeDelta);

	return 0;
}

_int CBoss3::Update_Top(const _float & fTimeDelta)
{
	if (-100.f < m_fAngle)
		m_pTransform->Rotation(ROT_X, D3DXToRadian(m_fAngle-- * fTimeDelta));

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

void CBoss3::FollowPlayer(const _float & fTimeDelta)
{
	m_fCoolDown += fTimeDelta;

	CGameObject* pGameObject = Engine::Get_GameObject(L"Layer_GameLogic", L"Topdee");
	NULL_CHECK_RETURN(pGameObject, );
	
	// 추격을 6초동안 진행하고
	if (3 < m_fCoolDown && 6 > m_fCoolDown)
	{
		m_pTransform->Chase_Target(&pGameObject->m_pTransform->m_vInfo[INFO_POS], m_fSpeed, fTimeDelta);
		m_pTransform->m_vInfo[INFO_POS].z = -2.f;
	}

	// 6초 후 공격 시작
	else if (6 < m_fCoolDown)
	{
		BossAttack(fTimeDelta);
	}
}

void CBoss3::BossAttack(const _float & fTimeDelta)
{
	static _float fAttackCoolDown = 0.f;
	fAttackCoolDown += fTimeDelta;

	// 회전하고 
	if(0.75f > fAttackCoolDown)
		m_pTransform->Rotation(ROT_Y, D3DXToRadian(270.f * fTimeDelta));

	// 내려 찍기
	else
	{
		if(5.f > m_pTransform->m_vInfo[INFO_POS].z)
			m_pTransform->m_vInfo[INFO_POS].z += 80.f * fTimeDelta; // 80.f 는 속도(상수)
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
