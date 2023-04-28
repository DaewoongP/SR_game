#include "stdafx.h"
#include "Boss2Stone.h"

CBoss2Stone::CBoss2Stone(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev), m_iIndex(0), m_fTimer(0.0f)
{
}

CBoss2Stone::~CBoss2Stone()
{
}

HRESULT CBoss2Stone::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };
	m_pRigid->m_bUseGrivaty = false;
	m_pTransform->m_vInfo[INFO_POS] = vPos;

	m_vStateSequence.push_back(&CBoss2Stone::Appear);
	m_vStateSequence.push_back(&CBoss2Stone::Aim);
	m_vStateSequence.push_back(&CBoss2Stone::Rest);
	m_vStateSequence.push_back(&CBoss2Stone::GoToTarget);
	m_vStateSequence.push_back(&CBoss2Stone::Dead);

	m_pToodeeTrans = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Toodee", L"Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pToodeeTrans, -1);
	m_pTopdeeTrans = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Topdee", L"Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(m_pTopdeeTrans, -1);

	return S_OK;
}

_int CBoss2Stone::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);
	(this->*m_vStateSequence[m_iIndex])(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

void CBoss2Stone::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CBoss2Stone::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();
	__super::Render_GameObject();
}

void CBoss2Stone::SwapTrigger()
{
}

void CBoss2Stone::OnCollisionEnter(const Collision * collision)
{
	__super::OnCollisionEnter(collision);
}

void CBoss2Stone::OnCollisionStay(const Collision * collision)
{
	__super::OnCollisionStay(collision);
}

void CBoss2Stone::OnCollisionExit(const Collision * collision)
{
	__super::OnCollisionExit(collision);
}

void CBoss2Stone::Appear(const _float & fTimeDelta)
{
	m_fTimer += fTimeDelta;
	if (1.0f < m_fTimer)
	{
		m_fTimer = 1.0f;
	}
	m_pTransform->m_vInfo[INFO_POS].y = Lerp(APPEAR_YPOS, FALLINGDEST_YPOS, m_fTimer);
	if (1.0f == m_fTimer)
	{
		m_fTimer = 0.0f;
		++m_iIndex;
	}
}

void CBoss2Stone::Aim(const _float & fTimeDelta)
{
	
}

void CBoss2Stone::Rest(const _float & fTimeDelta)
{
}

void CBoss2Stone::GoToTarget(const _float & fTimeDelta)
{
}

void CBoss2Stone::Dead(const _float & fTimeDelta)
{
}

HRESULT CBoss2Stone::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Boss2_Stone", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Texture", pComponent });

	pComponent = m_pRigid = dynamic_cast<CRigidbody*>(Engine::Clone_Proto(L"Rigidbody", this));
	NULL_CHECK_RETURN(m_pRigid, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Rigidbody", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });

	return S_OK;
}

CBoss2Stone * CBoss2Stone::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CBoss2Stone*		pInstance = new CBoss2Stone(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss2Stone::Free(void)
{
	__super::Free();
}
