#include "stdafx.h"
#include "Collider.h"
#include "GameObject.h"
#include "Transform.h"

#include "Export_Utility.h"

CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev) :
	CComponent(pGraphicDev)
	, m_pMesh(nullptr)
	, m_eGroup(COL_OBJ)
	, m_bIsTrigger(false)
	, m_pBoundingBox(nullptr)
{
}

CCollider::CCollider(const CCollider & rhs) :
	CComponent(rhs)
	, m_pBoundingBox(rhs.m_pBoundingBox)
	, m_pMesh(rhs.m_pMesh)
	, m_eGroup(rhs.m_eGroup)
	, m_bIsTrigger(rhs.m_bIsTrigger)
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
	D3DXMatrixTranslation(&m_matWorld,
		m_pBoundingBox->Get_Center().x,
		m_pBoundingBox->Get_Center().y,
		m_pBoundingBox->Get_Center().z);
	return 0;
}

void CCollider::LateUpdate_Component()
{
}

void CCollider::Render_Component()
{
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	m_pMesh->DrawSubset(0);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void CCollider::Insert_Collider(CCollider * pCollider, COL_DIR eDir)
{
	if (nullptr == pCollider)
		return;
	Collision* pCollision = nullptr;
	if (Find_ColList(pCollider, &pCollision))
	{
		pCollision->Set_Curcol(true);
		pCollision->otherCol = pCollider;
		pCollision->_dir = eDir;
	}
	else
	{
		pCollision = new Collision;
		pCollision->Set_Curcol(true);
		pCollision->otherCol = pCollider;
		pCollision->_dir = eDir;
		m_ColList.insert({ pCollider, pCollision });
	}
}

_bool CCollider::Find_ColList(CCollider * pOtherCol, Collision** collision)
{
	auto	iter = find_if(m_ColList.begin(), m_ColList.end(), [&](auto& iter)->_bool {
		return pOtherCol == iter.first;
	});

	if (iter == m_ColList.end())
		return false;
	
	collision = &iter->second;
	return true;
}

_bool CCollider::Delete_OtherCollider(CCollider * pOtherCol)
{
	auto	iter = find_if(m_ColList.begin(), m_ColList.end(), [&](auto& iter)->_bool {
		return pOtherCol == iter.first;
	});
	if (iter == m_ColList.end())
		return false;

	if (iter->second->Get_PreCol() == false)
	{
		delete iter->second;
		iter->second = nullptr;
		m_ColList.erase(iter);
		return false;
	}
	else
	{
		iter->second->Set_Curcol(false);
		return true;
	}
}

void CCollider::OnCollisionEnter(const Collision * collision)
{
	Change_ColliderColor(1.f, 0.f, 0.f, 1.f);
	m_pGameObject->OnCollisionEnter(collision);
}

void CCollider::OnCollisionStay(const Collision * collision)
{
	Change_ColliderColor(1.f, 0.f, 0.f, 1.f);
	m_pGameObject->OnCollisionStay(collision);
}

void CCollider::OnCollisionExit(const Collision * collision)
{
	Change_ColliderColor(0.f, 1.f, 0.f, 1.f);
	m_pGameObject->OnCollisionExit(collision);
}

// 가로, 세로, 깊이 사이즈 / 오프셋 좌표값 (객체 중점부터 상대좌표)
void CCollider::Set_BoundingBox(const _vec3 & vSize, const _vec3& vOffsetPos)
{
	D3DXCreateBox(m_pGraphicDev,
		vSize.x,
		vSize.y,
		vSize.z,
		&m_pMesh, NULL);

	Change_ColliderColor(0.f, 1.f, 0.f, 1.f);
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixTranslation(&m_matWorld, vOffsetPos.x, vOffsetPos.y, vOffsetPos.z);
  
	if (nullptr == m_pBoundingBox)
		m_pBoundingBox = new BoundingBox(-vSize / 2, vSize / 2, vOffsetPos);
	else
	{
		m_pBoundingBox->Set_Offset(vOffsetPos);
		m_pBoundingBox->_offsetMax = vSize / 2;
		m_pBoundingBox->_offsetMin = -vSize / 2;
		_vec3 offsetPoint;
		m_pGameObject->m_pTransform->Get_Info(INFO_POS, &offsetPoint);
		m_pBoundingBox->Offset(offsetPoint);
		
	}
}

HRESULT CCollider::Set_Options(const _vec3 & vSize, COLGROUP eGroup, _bool isTrigger)
{
	Set_BoundingBox(vSize);
	Set_Group(eGroup);
	m_bIsTrigger = isTrigger;
	return S_OK;
}

void CCollider::Set_Group(COLGROUP eGroup)
{
	Engine::Set_Collider(eGroup, this);
	m_eGroup = eGroup;
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
	Engine::Add_Collider(pClone);
	pClone->Set_BoundingBox();
	return pClone;
}

void CCollider::Free(void)
{
	Safe_Delete(m_pBoundingBox);
	for_each(m_ColList.begin(), m_ColList.end(), [](auto& iter) {
		delete iter.second;
		iter.second = nullptr;
	});
	m_ColList.clear();
	__super::Free();
}

void CCollider::Change_ColliderColor(_float r, _float g, _float b, _float a)
{
	m_pMesh->CloneMeshFVF(0, FVF_COL, m_pGraphicDev, &m_pMesh);

	m_pMesh->GetVertexBuffer(&m_pVB);
	_int iNum = m_pMesh->GetNumVertices();
	VTXCOL* pVertices = NULL;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);
	{
		for (int i = 0; i < iNum; ++i)
		{
			pVertices[i].dwColor = D3DXCOLOR(r, g, b, a);
		}
	}
	m_pVB->Unlock();
}
