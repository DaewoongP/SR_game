#include "stdafx.h"
#include "Collider.h"
#include "GameObject.h"
#include "Transform.h"

#include "Export_Function.h"
CCollider::CCollider(LPDIRECT3DDEVICE9 pGraphicDev) :
	CComponent(pGraphicDev)
	, m_pMesh(nullptr)
	, m_eGroup(COL_OBJ)
	, m_bIsTrigger(false)
	, m_pBoundingBox(nullptr)
	, m_pRedTex(nullptr)
	, m_pGreenTex(nullptr)
	, m_eColor(GREEN)
	, m_bRender(false)
{
}

CCollider::CCollider(const CCollider & rhs) :
	CComponent(rhs)
	, m_pBoundingBox(rhs.m_pBoundingBox)
	, m_pMesh(rhs.m_pMesh)
	, m_eGroup(rhs.m_eGroup)
	, m_bIsTrigger(rhs.m_bIsTrigger)
	, m_pRedTex(rhs.m_pRedTex)
	, m_pGreenTex(rhs.m_pGreenTex)
	, m_eColor(rhs.m_eColor)
	, m_bRender(rhs.m_bRender)
{
}

CCollider::~CCollider()
{
}

HRESULT CCollider::Ready_Collider()
{
	FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pGraphicDev, 
		L"../Resource/Texture/ColTex/Red.png", 
		(LPDIRECT3DTEXTURE9*)&m_pRedTex), E_FAIL);
	FAILED_CHECK_RETURN(D3DXCreateTextureFromFile(m_pGraphicDev,
		L"../Resource/Texture/ColTex/Green.png",
		(LPDIRECT3DTEXTURE9*)&m_pGreenTex), E_FAIL);
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
	if (Engine::Get_DIKeyState(DIK_F8) == Engine::KEYDOWN)
	{
		m_bRender = !m_bRender;
	}
}

void CCollider::Render_Component()
{
	if (!m_bRender)
		return;
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	DrawColor();
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
		Collision col;
		pCollision = &col;
		pCollision->Set_Curcol(true);
		pCollision->otherCol = pCollider;
		pCollision->_dir = eDir;
		m_ColList.insert({ pCollider, col });
	}
}

_bool CCollider::Find_ColList(CCollider * pOtherCol, Collision** collision)
{
	auto	iter = find_if(m_ColList.begin(), m_ColList.end(), [&](auto& iter)->_bool {
		return pOtherCol == iter.first;
	});

	if (iter == m_ColList.end())
		return false;
	
	*collision = &iter->second;
	return true;
}

_bool CCollider::Delete_OtherCollider(CCollider * pOtherCol)
{
	auto	iter = find_if(m_ColList.begin(), m_ColList.end(), [&](auto& iter)->_bool {
		return pOtherCol == iter.first;
	});
	if (iter == m_ColList.end())
		return false;

	if (iter->second.Get_PreCol() == false)
	{
		m_ColList.erase(iter);
		return false;
	}
	else
	{
		iter->second.Set_Curcol(false);
		return true;
	}
}

void CCollider::OnCollisionEnter(const Collision * collision)
{
	m_eColor = RED;
	m_pGameObject->OnCollisionEnter(collision);
}

void CCollider::OnCollisionStay(const Collision * collision)
{
	m_eColor = RED;
	m_pGameObject->OnCollisionStay(collision);
}

void CCollider::OnCollisionExit(const Collision * collision)
{
	m_eColor = GREEN;
	m_pGameObject->OnCollisionExit(collision);
}

// 가로, 세로, 깊이 사이즈 / 오프셋 좌표값 (객체 중점부터 상대좌표)
void CCollider::Set_BoundingBox(const _vec3 & vSize, const _vec3& vOffsetPos)
{
	Safe_Release(m_pMesh);
	D3DXCreateBox(m_pGraphicDev,
		vSize.x,
		vSize.y,
		vSize.z,
		&m_pMesh, NULL);

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

void CCollider::DrawColor()
{
	switch (m_eColor)
	{
	case Engine::RED:
		m_pGraphicDev->SetTexture(0, m_pRedTex);
		break;
	case Engine::GREEN:
		m_pGraphicDev->SetTexture(0, m_pGreenTex);
		break;
	default:
		m_pGraphicDev->SetTexture(0, m_pGreenTex);
		break;
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
	m_ColList.clear();
	__super::Free();
}
