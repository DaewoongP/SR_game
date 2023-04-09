#include "stdafx.h"
#include "KeyBox.h"

#include "Export_Function.h"
#include "Key.h"

CKeyBox::CKeyBox(LPDIRECT3DDEVICE9 pGraphicDev):CCube(pGraphicDev)
{

}

CKeyBox::~CKeyBox()
{
}

HRESULT CKeyBox::Ready_GameObject(_vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vScale = { 1.f,1.f,1.f };
	
	return S_OK;
}

_int CKeyBox::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	__super::Update_GameObject(fTimeDelta);
	return S_OK;
}

void CKeyBox::LateUpdate_GameObject(void)
{
	if (CKey::iKeyCnt == 0)
		m_bDead = true;
	__super::LateUpdate_GameObject();
}

void CKeyBox::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture();

	m_pBufferCom->Render_Buffer();

	CGameObject::Render_GameObject();
}

HRESULT CKeyBox::Add_Component(void)
{
	CComponent* pComponent = nullptr;
	
	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"CubeTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"CubeTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"KeyBox_Cube", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"KeyBox_Cube", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Collider", pComponent });
	m_pCollider->Set_BoundingBox({ 2.f, 2.f,2.f });
	m_pCollider->Set_Group(COL_ENV);

	return S_OK;
}

CKeyBox* CKeyBox::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos)
{
	CKeyBox* pInstance = new CKeyBox(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	
	return pInstance;
}

void CKeyBox::Free()
{
	__super::Free();
}
