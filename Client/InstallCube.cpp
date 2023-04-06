#include "stdafx.h"
#include "InstallCube.h"

#include "Export_Function.h"
CInstallCube::CInstallCube(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCube(pGraphicDev),
	m_iCubeIndex(0)
{
}

CInstallCube::~CInstallCube()
{
}

HRESULT CInstallCube::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };
	return S_OK;
}

_int CInstallCube::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	CGameObject::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CInstallCube::LateUpdate_GameObject(void)
{
	CGameObject::LateUpdate_GameObject();
}

void CInstallCube::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(m_iCubeIndex);

	m_pBufferCom->Render_Buffer();

	CGameObject::Render_GameObject();
}

void CInstallCube::OnCollisionEnter(const Collision * collision)
{
	CGameObject::OnCollisionEnter(collision);
}

void CInstallCube::OnCollisionStay(const Collision * collision)
{
	CGameObject::OnCollisionStay(collision);
}

void CInstallCube::OnCollisionExit(const Collision * collision)
{
	CGameObject::OnCollisionExit(collision);
}

HRESULT CInstallCube::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"CubeTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"CubeTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Stage1_Cube", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Stage1_Cube", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Collider", pComponent });
	m_pCollider->Set_BoundingBox({ 2.f,2.f,2.f });
	m_pCollider->Set_Group(COL_ENV);

	return S_OK;
}

CInstallCube * CInstallCube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CInstallCube*		pInstance = new CInstallCube(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CInstallCube::Free(void)
{
	CGameObject::Free();
}
