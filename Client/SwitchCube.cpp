#include "stdafx.h"
#include "SwitchCube.h"

#include "Export_Function.h"
#include "Switch.h"

CSwitchCube::CSwitchCube(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCube(pGraphicDev)
{
}

CSwitchCube::~CSwitchCube()
{
}

HRESULT CSwitchCube::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vScale = { 1.f,1.f,1.f };
	m_pTransform->m_bIsStatic = true;

	m_pCollider->Set_BoundingBox({ 2.f, 2.f,2.f });
	m_pCollider->Set_Group(COL_ENV);

	return S_OK;
}

_int CSwitchCube::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (!CSwitch::m_bSwtichON)
	{
		m_pTransform->m_bIsStatic = true;
		m_pCollider->Set_BoundingBox(_vec3(2, 2, 2));
	}

	if (CSwitch::m_bSwtichON)
	{
		m_pTransform->m_bIsStatic = false;
		m_pCollider->Set_BoundingBox(_vec3(0, 0, 0));
	}

	__super::Update_GameObject(fTimeDelta);

	return S_OK;
}

void CSwitchCube::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CSwitchCube::Render_GameObject(void)
{
	if (!CSwitch::m_bSwtichON)
	{
		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

		m_pTextureCom->Set_Texture();

		m_pBufferCom->Render_Buffer();
	}
	
	CGameObject::Render_GameObject();
}

void CSwitchCube::OnCollisionEnter(const Collision * collision)
{
	//if (CSwitch::m_bSwtichON)
	//{
	//	if(!lstrcmp(L"Toodee", collision->otherObj->m_pTag) || 
	//		!lstrcmp(L"Pig", collision->otherObj->m_pTag)||
	//		!lstrcmp(L"Boss2", collision->otherObj->m_pTag))
	//		
	//}
}

HRESULT CSwitchCube::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"CubeTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"CubeTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"SwitchCube_Cube", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"SwitchCube_Cube", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });

	return S_OK;
}

CSwitchCube * CSwitchCube::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CSwitchCube* pInstance = new CSwitchCube(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSwitchCube::Free()
{
	__super::Free();
}
