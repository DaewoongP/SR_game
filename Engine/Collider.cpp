#include "stdafx.h"
#include "Collider.h"
#include "GameObject.h"
#include "Transform.h"

#include "Export_Utility.h"

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev) :
	CComponent(pGraphicDev),
	m_bEnabled(true),
	m_bIsTrigger(false),
	m_bIsCollision(false),
	m_pMesh(nullptr)
{
}

CCollider::CCollider(const CCollider & rhs) :
	CComponent(rhs),
	m_bEnabled(rhs.m_bEnabled),
	m_bIsTrigger(rhs.m_bIsTrigger),
	m_pBoundingBox(rhs.m_pBoundingBox),
	m_bIsCollision(rhs.m_bIsCollision)
	,m_pMesh(rhs.m_pMesh)
{
}

CCollider::~CCollider()
{
}

HRESULT CCollider::Ready_Collider(_bool bIsTrigger)
{
	m_bIsTrigger = bIsTrigger;
	m_bIsCollision = !bIsTrigger;

	return S_OK;
}

_int CCollider::Update_Component(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Component(fTimeDelta);
	if (iExit != 0) return iExit;

	_vec3 offsetPoint;
	m_pGameObject->m_pTransform->Get_Info(INFO_POS, &offsetPoint);
	m_pBoundingBox->Offset(offsetPoint);

	return 0;
}

void CCollider::LateUpdate_Component()
{
	
}

void CCollider::Render_Component()
{
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pMesh->DrawSubset(0);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void CCollider::OnCollisionEnter(const Collision * collision)
{
	m_pGameObject->OnCollisionEnter(collision);
}

void CCollider::OnCollisionStay(const Collision * collision)
{
	m_pGameObject->OnCollisionStay(collision);
}

void CCollider::OnCollisionExit(const Collision * collision)
{
	m_pGameObject->OnCollisionExit(collision);
}

void CCollider::OnTriggerEnter(const CCollider * other)
{
	m_pGameObject->OnTriggerEnter(other);
}

void CCollider::OnTriggerStay(const CCollider * other)
{
	m_pGameObject->OnTriggerStay(other);
}

void CCollider::OnTirggerExit(const CCollider * other)
{
	m_pGameObject->OnTirggerExit(other);
}

// 가로, 세로, 깊이 사이즈 넣어주면 됨.
void CCollider::Set_BoundingBox(const _vec3 & vSize)
{
	D3DXCreateBox(m_pGraphicDev,
		vSize.x,
		vSize.y,
		vSize.z,
		&m_pMesh, NULL);
	if (nullptr == m_pBoundingBox)
		m_pBoundingBox = new BoundingBox(-vSize / 2, vSize / 2);
	else
	{
		m_pBoundingBox->_offsetMax = vSize / 2;
		m_pBoundingBox->_offsetMin = -vSize / 2;
		_vec3 offsetPoint;
		m_pGameObject->m_pTransform->Get_Info(INFO_POS, &offsetPoint);
		m_pBoundingBox->Offset(offsetPoint);
	}
}

CCollider * CCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev, _bool bIsTrigger)
{
	CCollider* pInstance = new CCollider(pGraphicDev);

	if (FAILED(pInstance->Ready_Collider(bIsTrigger)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CCollider::Clone(void)
{
	CCollider* pClone = new CCollider(*this);
	Engine::Add_Collider(COL_OBJ, pClone);
	pClone->Set_BoundingBox();
	return pClone;
}

void CCollider::Free(void)
{
	Safe_Delete(m_pBoundingBox);
	__super::Free();
}
