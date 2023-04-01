#include "stdafx.h"
#include "Grid.h"

#include "Export_Function.h"

CGrid::CGrid(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CGrid::~CGrid()
{
}

HRESULT CGrid::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CGrid::Update_GameObject(const _float & fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CGrid::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CGrid::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pBufferCom->Render_Buffer();
}

HRESULT CGrid::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcCol*>(Engine::Clone_Proto(L"RcCol", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"RcCol", pComponent });

	return S_OK;
}

CGrid * CGrid::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGrid*		pInstance = new CGrid(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CGrid::Free(void)
{
	__super::Free();
}
