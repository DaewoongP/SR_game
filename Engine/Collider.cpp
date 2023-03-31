#include "stdafx.h"
#include "Collider.h"
#include "GameObject.h"
#include "Transform.h"

#include "Export_Utility.h"

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev) :
	CComponent(pGraphicDev)
	, m_pMesh(nullptr)
{
}

CCollider::CCollider(const CCollider & rhs) :
	CComponent(rhs)
	, m_pBoundingBox(rhs.m_pBoundingBox)
	, m_pMesh(rhs.m_pMesh)
{
}

CCollider::~CCollider()
{
}

HRESULT CCollider::Ready_Collider()
{

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

void CCollider::Add_Collider(CCollider * pCollider, COL_DIR eDir)
{
	if (nullptr == pCollider)
		return;

	if (Check_AlreadyCol(pCollider))
	{
		Collision* pColl = Find_ColState(pCollider);
		COL_STATE eState = pColl->Set_Curcol(true);
		pColl->_dir = eDir;
	}
	else
	{
		Collision tCollision;
		tCollision.Set_Curcol(true);
		tCollision._dir = eDir;
		m_Colmap.insert({ pCollider, &tCollision });
	}
}

Collision * CCollider::Find_ColState(CCollider * pOtherCol)
{
	auto	iter = find_if(m_Colmap.begin(), m_Colmap.end(), [&](auto& iter)->_bool {
		return pOtherCol == iter.first;
	});

	if (iter == m_Colmap.end())
		return nullptr;

	return iter->second;
}

_bool CCollider::Check_AlreadyCol(CCollider * pOtherCol)
{
	auto	iter = find_if(m_Colmap.begin(), m_Colmap.end(), [&](auto& iter)->_bool {
		return pOtherCol == iter.first;
	});

	if (iter == m_Colmap.end())
		return false;
	return true;
}

void CCollider::Delete_Collider(CCollider * pCollider)
{
	/*auto	iter = find_if(m_Colmap.begin(), m_Colmap.end(), [&](auto& iter)->_bool {
		return pOtherCol == iter.first;
	});*/
	
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

	if (FAILED(pInstance->Ready_Collider()))
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
