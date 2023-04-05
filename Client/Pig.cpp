#include "stdafx.h"
#include "Pig.h"
#include "Cube.h"
#include "Export_Function.h"
#include "PigTail.h"

CPig::CPig(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev),m_bMoveLeft(false)
{
}

CPig::~CPig()
{
}

HRESULT CPig::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fSpeed = 8.0f;
	m_pTransform->m_vScale = { -2.f, 2.f, 2.f };
	m_pTransform->m_vInfo[INFO_POS] = _vec3(50.f, 7.f, 10.f);
	m_pTransform->m_bIsStatic = false;
	
	return S_OK;
}

_int CPig::Update_GameObject(const _float & fTimeDelta)
{	
	if (m_bDead)
		return OBJ_DEAD;
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	m_pTextureCom->Update_Anim(fTimeDelta);
	__super::Update_GameObject(fTimeDelta);
	return 0;
}

void CPig::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CPig::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	__super::Render_GameObject();
}

HRESULT CPig::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Pig_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Pig_Texture", pComponent });
	m_pTextureCom->Add_Anim(L"Idle", 0, 8, 1.f, true);
	m_pTextureCom->Switch_Anim(L"Idle");
	m_pTextureCom->m_bUseFrameAnimation = true;

	pComponent = m_pRigid = dynamic_cast<CRigidbody*>(Engine::Clone_Proto(L"Rigidbody", this));
	NULL_CHECK_RETURN(m_pRigid, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Rigidbody", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Collider", pComponent });
	m_pCollider->Set_BoundingBox({ 1.0f, 1.0f, 0.1f });

	m_pTransform->m_bIsStatic = false;

	return S_OK;
}

_int CPig::Update_Too(const _float & fTimeDelta)
{
	_vec3 MoveDir = { 1.0f,0.0f,0.0f };

	m_pRigid->m_bUseGrivaty = true;

	if (!m_bMoveLeft)
	{
		MoveDir *= -1;
	}

	m_pTransform->Move_Pos(&MoveDir, fTimeDelta, m_fSpeed);

	return 0;
}

_int CPig::Update_Top(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	CTransform*	pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransformCom, -1);

	m_pRigid->m_bUseGrivaty = false;

	_vec3	vPlayerPos;

	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	vPlayerPos = vPlayerPos - m_pTransform->m_vInfo[INFO_POS];

	D3DXVec3Normalize(&vPlayerPos, &vPlayerPos);

	m_pTransform->Move_Pos(&vPlayerPos, fTimeDelta, m_fSpeed);

	//z축 회전
	_vec3 vStandard = { 1.0f,0.0f,0.0f };

	m_pTransform->m_vAngle.z = (acosf(D3DXVec3Dot(&vStandard, &vPlayerPos)));

	if (0>vPlayerPos.y)
	{
		m_pTransform->m_vAngle.z = 2 * D3DX_PI - m_pTransform->m_vAngle.z;
	}
	
	return 0;
}


void CPig::OnCollisionEnter(const Collision * collision)
{
	if ((collision->_dir == DIR_LEFT || collision->_dir == DIR_RIGHT) && dynamic_cast<CCube*>(collision->otherObj))
	{
		if (m_bMoveLeft)
		{
			m_bMoveLeft = false;
		}
		else if (!m_bMoveLeft)
		{
			m_bMoveLeft = true;
		}
		m_pTransform->m_vScale.x *= -1;
	}
	__super::OnCollisionEnter(collision);
}

void CPig::OnCollisionStay(const Collision * collision)
{
	__super::OnCollisionStay(collision);
}

CPig * CPig::Create(LPDIRECT3DDEVICE9 pGraphicDev, CLayer* pLayer)
{
	CPig*		pInstance = new CPig(pGraphicDev);
	CGameObject*	pPartObjects  = nullptr;

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	// 파츠 구현부
	pPartObjects = CPigTail::Create(pGraphicDev, pInstance->m_pTransform);
	NULL_CHECK(pPartObjects);
	pLayer->Add_GameObject(L"PigTail_0", pPartObjects);

	return pInstance;
}

void CPig::Free(void)
{
	__super::Free();
}
