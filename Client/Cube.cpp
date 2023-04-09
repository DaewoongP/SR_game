#include "stdafx.h"
#include "Cube.h"
#include "Export_Function.h"
#include "GravityCube.h"
CCube::CCube(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CCube::~CCube()
{
}

HRESULT CCube::Ready_GameObject(_vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };

	m_pCollider->Set_BoundingBox({ 2.f,2.f,2.f });
	m_pCollider->Set_Group(COL_ENV);
	return S_OK;
}

_int CCube::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	__super::Update_GameObject(fTimeDelta);
  
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CCube::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CCube::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture();

	m_pBufferCom->Render_Buffer();

	__super::Render_GameObject();
}

void CCube::OnCollisionEnter(const Collision * collision)
{
	__super::OnCollisionEnter(collision);
}

void CCube::OnCollisionStay(const Collision * collision)
{
	__super::OnCollisionStay(collision);
	if (dynamic_cast<CGravityCube*>(collision->otherObj) &&
		collision->_dir == DIR_UP)
	{
		collision->otherObj->m_pTransform->m_bIsStatic = true;
	}
}

void CCube::OnCollisionExit(const Collision * collision)
{
	__super::OnCollisionExit(collision);
	if (dynamic_cast<CGravityCube*>(collision->otherObj) &&
		collision->_dir == DIR_UP)
	{
		collision->otherObj->m_pTransform->m_bIsStatic = false;
	}
}

HRESULT CCube::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"CubeTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"CubeTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Stage1_Cube",this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Stage1_Cube", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Collider", pComponent });

	return S_OK;
}

CCube * CCube::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos)
{
	CCube*		pInstance = new CCube(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CCube::Free(void)
{
	__super::Free();
}
