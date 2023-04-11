#include "stdafx.h"
#include "PortalCube.h"
#include "Export_Function.h"
#include "Toodee.h"

CPortalCube::CPortalCube(LPDIRECT3DDEVICE9 pGraphicDev) :CMoveCube(pGraphicDev)
{
}

CPortalCube::~CPortalCube()
{
}

HRESULT CPortalCube::Ready_GameObject(_vec3 & vPos)
{
	__super::Ready_GameObject(vPos);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_bIsStatic = true;
	m_pCollider->Set_Options();
	// Up vector = portal col direction
	// COL dir == CD dir
	_matrix matInfo;
	D3DXMatrixIdentity(&matInfo);
	
	switch (m_eDir)
	{
	case Engine::CD_UP:
		break;
	case Engine::CD_LEFT:
		m_pTransform->Rotation(ROT_Z, D3DXToRadian(90));
		break;
	case Engine::CD_DOWN:
		m_pTransform->Rotation(ROT_Z, D3DXToRadian(180));
		break;
	case Engine::CD_RIGHT:
		m_pTransform->Rotation(ROT_Z, D3DXToRadian(270));
		break;
	}
	m_bInit = true;
	return S_OK;
}

_int CPortalCube::Update_Too(const _float & fTimeDelta)
{
	if (m_bInit)
	{
		m_pOtherCube = Get_GameObject(L"Layer_GameLogic", L"PortalCube");
		if (m_pOtherCube != this)
			static_cast<CPortalCube*>(m_pOtherCube)->Set_Other(this);
		m_bInit = false;
	}
	return 0;
}

_int CPortalCube::Update_Top(const _float & fTimeDelta)
{
	__super::Update_Top(fTimeDelta);
	return 0;
}

_int CPortalCube::Update_GameObject(const _float & fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	return 0;
}

void CPortalCube::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CPortalCube::Render_GameObject(void)
{
	__super::Render_GameObject();
}

_vec3 CPortalCube::Trans_Velocity(_vec3 & velocity, CPortalCube* other)
{
	_vec3 vNormal, vRref, outVelocity, outUp;
	_matrix matRot;
	_float fSpeed, fRad;
	fSpeed = D3DXVec3Length(&velocity);
	D3DXVec3Normalize(&vNormal, &velocity);
	fRad = acosf(D3DXVec3Dot(&(-m_pTransform->m_vInfo[INFO_UP]), &vNormal));
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixRotationZ(&matRot, fRad);
	switch (other->m_eDir)
	{
	case Engine::CD_UP:
		outUp = _vec3(0.f, 1.f, 0.f);
		break;
	case Engine::CD_DOWN:
		outUp = _vec3(0.f, -1.f, 0.f);
		break;
	case Engine::CD_LEFT:
		outUp = _vec3(-1.f, 0.f, 0.f);
		break;
	case Engine::CD_RIGHT:
		outUp = _vec3(1.f, 0.f, 0.f);
		break;
	}

	D3DXVec3TransformNormal(&vRref, &outUp, &matRot);
	outVelocity = vRref * fSpeed;
	return outVelocity;
}

void CPortalCube::OnCollisionEnter(const Collision * collision)
{
	if ((_int)collision->_dir == (_int)m_eDir && 
		lstrcmp(collision->otherObj->m_pTag, L"MapCube") &&
		lstrcmp(collision->otherObj->m_pTag, L"PortalCube") &&
		lstrcmp(collision->otherObj->m_pTag, L"Topdee"))
	{
		collision->otherObj->m_pTransform->m_vInfo[INFO_POS] = static_cast<CPortalCube*>(m_pOtherCube)->Get_CubeHeadPos();
		if (!lstrcmp(collision->otherObj->m_pTag, L"Toodee"))
		{
			CComponent* pOtherRigid = Engine::Get_Component(L"Layer_GameLogic", L"Toodee", L"Rigidbody", ID_DYNAMIC);
			_vec3 velocity = dynamic_cast<CRigidbody*>(pOtherRigid)->m_Velocity;
			dynamic_cast<CRigidbody*>(pOtherRigid)->m_Velocity = Trans_Velocity(velocity, static_cast<CPortalCube*>(m_pOtherCube));
		}
	}
	__super::OnCollisionEnter(collision);
}

void CPortalCube::OnCollisionStay(const Collision * collision)
{
	__super::OnCollisionStay(collision);
}

void CPortalCube::OnCollisionExit(const Collision * collision)
{
	__super::OnCollisionExit(collision);
}

_vec3 CPortalCube::Get_CubeHeadPos()
{
	switch (m_eDir)
	{
	case Engine::CD_UP:
		return m_pTransform->m_vInfo[INFO_POS] + _vec3(0.f, 2.f, 0.f);
	case Engine::CD_DOWN:
		return m_pTransform->m_vInfo[INFO_POS] + _vec3(0.f, -2.f, 0.f);
	case Engine::CD_LEFT:
		return m_pTransform->m_vInfo[INFO_POS] + _vec3(-2.f, 0.f, 0.f);
	case Engine::CD_RIGHT:
		return m_pTransform->m_vInfo[INFO_POS] + _vec3(2.f, 0.f, 0.f);
	default:
		return _vec3(0.f, 0.f, 0.f);
	}
}


HRESULT CPortalCube::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Portal_Cube", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Portal_Cube", pComponent });

	return S_OK;
}


CPortalCube * CPortalCube::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos, int eDir)
{
	CPortalCube*		pInstance = new CPortalCube(pGraphicDev);

	pInstance->Set_CubeDir((CUBE_DIR)eDir);
	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CPortalCube::Free(void)
{
	__super::Free();
}

