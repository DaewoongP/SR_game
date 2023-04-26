#include "stdafx.h"
#include "Item.h"
#include "Export_Function.h"

CItem::CItem(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev),
	m_iIndex(0)
{
}

CItem::~CItem()
{
}

HRESULT CItem::Ready_GameObject(_vec3 & vPos, _int iIndex)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vScale = { 2.f, 2.f, 2.f };
	m_pTransform->m_bIsStatic = false;

	m_iIndex = iIndex;

	return S_OK;
}

_int CItem::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	CGameObject::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_NONE, this);

	return 0;
}

void CItem::LateUpdate_GameObject(void)
{
	CGameObject::LateUpdate_GameObject();
}

void CItem::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(m_iIndex);

	m_pBufferCom->Render_Buffer();
}

void CItem::OnCollisionEnter(const Collision * collision)
{
	if (!lstrcmp(collision->otherObj->m_pTag, L"Toodee") ||
		!lstrcmp(collision->otherObj->m_pTag, L"Topdee"))
		m_bDead = true;

   	CGameObject::OnCollisionEnter(collision);
}

HRESULT CItem::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Item_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Item_Texture", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });
	m_pCollider->Set_BoundingBox(_vec3{ 2.f, 2.f, 2.f });
	m_pCollider->Set_Group(COL_OBJ);

	return S_OK;
}

CItem * CItem::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos, _int iIndex)
{
	CItem* pInstance = new CItem(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, iIndex)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CItem::Free(void)
{
	__super::Free();
}
