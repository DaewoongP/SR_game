#include "stdafx.h"
#include "KeyBox.h"

#include"..\Engine\Export_Function.h"

CKeyBox::CKeyBox(LPDIRECT3DDEVICE9 pGraphicDev):CCube(pGraphicDev)
{
}

CKeyBox::~CKeyBox()
{
}

HRESULT CKeyBox::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vScale = { 1.f,1.f,1.f };
	m_pTransform->m_vInfo[INFO_POS] = _vec3(35.f, 20.f, 10.f);
	m_pCollider->Set_Group(COL_OBJ);
	
	return S_OK;
}

_int CKeyBox::Update_GameObject(const _float& fTimeDelta)
{
	

	return S_OK();
}

_int CKeyBox::Update_Too(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

_int CKeyBox::Update_Top(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CKeyBox::LateUpdate_GameObject(void)
{
}

void CKeyBox::LateUpdate_Too()
{
	// 키카운트 스테이지에서 지정해주고 감소해서 0이되면 사라지게하는게 좋을듯
	//Key부분에서 Add대신 DisCount로 바꾸는것이좋아보임
	m_iKey_Count;
	if (CGameObject::Get_KeyCount() == 0)
		m_bDead = true;
	CGameObject::LateUpdate_GameObject();

}

void CKeyBox::LateUpdate_Top()
{

	
	if (CGameObject::Get_KeyCount()==0)
		m_bDead = true;
	CGameObject::LateUpdate_GameObject();

}

void CKeyBox::Render_GameObject(void)
{
}

void CKeyBox::Render_Too()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture();

	m_pBufferCom->Render_Buffer();

	CGameObject::Render_GameObject();
}

void CKeyBox::Render_Top()
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
	m_pCollider->Set_BoundingBox({ 1.99f,1.99f,1.99f });
	m_pCollider->Set_Group(COL_ENV);

	return S_OK;
}

CKeyBox* CKeyBox::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CKeyBox* pInstance = new CKeyBox(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject()))
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
