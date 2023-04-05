#include "stdafx.h"
#include "InstallGrid.h"

#include "Export_Function.h"

CInstallGrid::CInstallGrid(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGrid(pGraphicDev),
	m_iStageTileIndex(0)
{
}

CInstallGrid::~CInstallGrid()
{
}

HRESULT CInstallGrid::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	return S_OK;
}

_int CInstallGrid::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	CGameObject::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CInstallGrid::LateUpdate_GameObject(void)
{
	CGameObject::LateUpdate_GameObject();
}

void CInstallGrid::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(m_iStageTileIndex);

	m_pBufferCom->Render_Buffer();
}

HRESULT CInstallGrid::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Stage1_Tile_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Stage1_Tile_Texture", pComponent });

	return S_OK;
}

CInstallGrid * CInstallGrid::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CInstallGrid* pInstance = new CInstallGrid(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		pInstance = nullptr;
	}

	return pInstance;
}

void CInstallGrid::Free(void)
{
	CGameObject::Free();
}
