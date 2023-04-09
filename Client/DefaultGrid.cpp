#include "stdafx.h"
#include "DefaultGrid.h"

#include "Export_Function.h"

CDefaultGrid::CDefaultGrid(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGrid(pGraphicDev),
	m_iStageTileIndex(0)
{
}

CDefaultGrid::~CDefaultGrid()
{
}

HRESULT CDefaultGrid::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS] = vPos;

	return S_OK;
}

_int CDefaultGrid::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	Key_Input();

	CGameObject::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CDefaultGrid::LateUpdate_GameObject(void)
{
	CGameObject::LateUpdate_GameObject();
}

void CDefaultGrid::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(m_iStageTileIndex);

	m_pBufferCom->Render_Buffer();
}

HRESULT CDefaultGrid::Add_Component(void)
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

void CDefaultGrid::Key_Input()
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	_int x = 1366 / CUBEX * 2;
	_int y = 768 / CUBEY * 2;

	_int nx = 1366 / x;
	_int ny = 768 / y;

	_float Gridx = pt.x / nx / (1.3f) - 1.f;
	_float Gridy = (768 / ny - pt.y / ny) / (2.35f) - 1.f;

	_int installx = _int(Gridx / 2);
	_int instally = _int(Gridy / 2);

	m_pTransform->m_vInfo[INFO_POS].x = installx * 2.f;
	m_pTransform->m_vInfo[INFO_POS].y = instally * 2.f;
	m_pTransform->m_vInfo[INFO_POS].z = 10.f;
}

CDefaultGrid * CDefaultGrid::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CDefaultGrid* pInstance = new CDefaultGrid(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		pInstance = nullptr;
	}

	return pInstance;
}

void CDefaultGrid::Free(void)
{
	CGameObject::Free();
}