#include "stdafx.h"
#include "InvisibleGrid.h"

#include "Export_Function.h"

CInvisibleGrid::CInvisibleGrid(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CInvisibleGrid::~CInvisibleGrid()
{
}

HRESULT CInvisibleGrid::Ready_GameObject(_vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;

	return S_OK;
}

_int CInvisibleGrid::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	CGameObject::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CInvisibleGrid::LateUpdate_GameObject(void)
{
	CGameObject::LateUpdate_GameObject();
}

void CInvisibleGrid::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	CGameObject::Render_GameObject();
}

HRESULT CInvisibleGrid::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });
	m_pCollider->Set_BoundingBox(_vec3{ 2.f, 2.f, 2.f });
	//m_pCollider->Set_Group();

	return S_OK;
}

CInvisibleGrid * CInvisibleGrid::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos)
{
	CInvisibleGrid* pInstance = new CInvisibleGrid(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		pInstance = nullptr;
	}

	return pInstance;
}

void CInvisibleGrid::Free(void)
{
	CGameObject::Free();
}
