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

HRESULT CDefaultGrid::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

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
	if (Engine::Get_DIKeyState(DIK_A) == Engine::KEYDOWN) // ��
		m_pTransform->m_vInfo[INFO_POS].x -= 2.f;

	if (Engine::Get_DIKeyState(DIK_D) == Engine::KEYDOWN) // ��
		m_pTransform->m_vInfo[INFO_POS].x += 2.f;

	if (Engine::Get_DIKeyState(DIK_W) == Engine::KEYDOWN) // ��
		m_pTransform->m_vInfo[INFO_POS].y += 2.f;

	if (Engine::Get_DIKeyState(DIK_S) == Engine::KEYDOWN) // ��
		m_pTransform->m_vInfo[INFO_POS].y -= 2.f;
}

CDefaultGrid * CDefaultGrid::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDefaultGrid* pInstance = new CDefaultGrid(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
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
