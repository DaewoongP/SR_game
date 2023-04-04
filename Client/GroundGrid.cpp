#include "stdafx.h"
#include "GroundGrid.h"

#include "Export_Function.h"

CGroundGrid::CGroundGrid(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGrid(pGraphicDev)
{
}

CGroundGrid::~CGroundGrid()
{
}

HRESULT CGroundGrid::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CGroundGrid::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	CGameObject::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CGroundGrid::LateUpdate_GameObject(void)
{
	CGameObject::LateUpdate_GameObject();
}

void CGroundGrid::Render_GameObject(void)
{
	if (m_bGridON)
	{
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
		//m_pGraphicDev->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

		m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

		m_pTextureCom->Set_Texture();

		m_pBufferCom->Render_Buffer();

		m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		//m_pGraphicDev->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);

		m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}	
}

HRESULT CGroundGrid::Add_Component(void)
{
	CComponent*		pComponent = nullptr;
	
	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"RcTex", pComponent });
	
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Tile_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Tile_Texture", pComponent });
	
	return S_OK;
}

CGroundGrid * CGroundGrid::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CGroundGrid* pInstance = new CGroundGrid(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		pInstance = nullptr;
	}

	return pInstance;
}

void CGroundGrid::Free(void)
{
	CGameObject::Free();
}
