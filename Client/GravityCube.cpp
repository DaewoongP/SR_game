#include "stdafx.h"
#include "GravityCube.h"
#include "Export_Function.h"

CGravityCube::CGravityCube(LPDIRECT3DDEVICE9 pGraphicDev):CMoveCube(pGraphicDev)
{
}

CGravityCube::~CGravityCube()
{
}

HRESULT CGravityCube::Ready_GameObject(_vec3 & vPos)
{
	__super::Ready_GameObject(vPos);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pRigid->m_bUseGrivaty = true;
	m_pTransform->m_bIsStatic = false;
	m_pCollider->m_bIsTrigger = false;
	m_pCollider->Set_BoundingBox({ 2.f,2.f,2.f });
	return S_OK;
}

_int CGravityCube::Update_GameObject(const _float & fTimeDelta)
{
	__super::Update_GameObject(0.02f);
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	return 0;
}

_int CGravityCube::Update_Too(const _float & fTimeDelta)
{
	__super::Update_Too(fTimeDelta);
	if (!m_bIsStone)
		m_pRigid->m_bUseGrivaty = true;
	else
		m_pRigid->m_bUseGrivaty = false;
	return 0;
}

_int CGravityCube::Update_Top(const _float & fTimeDelta)
{
	__super::Update_Top(fTimeDelta);
	m_pRigid->m_bUseGrivaty = false;
	m_pRigid->m_Velocity = _vec3(0, 0, 0);
	return 0;
}

void CGravityCube::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CGravityCube::Render_GameObject(void)
{
	__super::Render_GameObject();
}

void CGravityCube::OnCollisionEnter(const Collision * collision)
{
	__super::OnCollisionEnter(collision);
}

void CGravityCube::OnCollisionStay(const Collision * collision)
{
	__super::OnCollisionStay(collision);
	if (dynamic_cast<CCube*>(collision->otherObj)&&
		collision->_dir == DIR_DOWN)
	{
		m_pRigid->m_bUseGrivaty = false;
		m_pRigid->m_Velocity = _vec3(0, 0, 0);
	}
}

void CGravityCube::OnCollisionExit(const Collision * collision)
{
	if (dynamic_cast<CCube*>(collision->otherObj) &&
		collision->_dir == DIR_DOWN)
	{
		//m_pTransform->m_bIsStatic = false;
	}
	__super::OnCollisionExit(collision);
}

CGravityCube * CGravityCube::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CGravityCube*		pInstance = new CGravityCube(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CGravityCube::Free(void)
{
	__super::Free();
}

HRESULT CGravityCube::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pRigid = dynamic_cast<CRigidbody*>(Engine::Clone_Proto(L"Rigidbody", this));
	NULL_CHECK_RETURN(m_pRigid, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Rigidbody", pComponent });
	return S_OK;
}
