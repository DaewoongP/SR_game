#include "stdafx.h"
#include "DefaultGrid.h"

#include "Export_Function.h"

CDefaultGrid::CDefaultGrid(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev),
	m_iStageTileIndex(0), 
	m_bGridON(true)
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
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Stage1_Tile_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Stage1_Tile_Texture", pComponent });

	return S_OK;
}

void CDefaultGrid::Key_Input()
{
	POINT	pt{};
	GetCursorPos(&pt);
	ScreenToClient(g_hWnd, &pt);

	D3DVIEWPORT9		ViewPort;
	ZeroMemory(&ViewPort, sizeof(D3DVIEWPORT9));
	// 뷰포트 가져오는 함수
	m_pGraphicDev->GetViewport(&ViewPort);

	_vec3	vMouse;
	vMouse.x = pt.x / (ViewPort.Width * 0.5f) - 1.f;
	vMouse.y = pt.y / -(ViewPort.Height * 0.5f) + 1.f;
	vMouse.z = 0.f;

	_matrix		matProj;
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);
	D3DXMatrixInverse(&matProj, 0, &matProj);
	D3DXVec3TransformCoord(&vMouse, &vMouse, &matProj);

	_matrix		matView;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	D3DXMatrixInverse(&matView, 0, &matView);

	_vec3	vRayPos, vRayDir;
	vRayPos = { 0.f,0.f,0.f };
	vRayDir = vMouse - vRayPos;
	D3DXVec3TransformCoord(&vRayPos, &vRayPos, &matView);
	D3DXVec3TransformNormal(&vRayDir, &vRayDir, &matView);

	vRayDir /= vRayDir.z;

	_vec3 vPos = vRayPos + vRayDir * (10 - matView._43);

	if ((_int)ceil(vPos.x) % 2 == 0)
		vPos.x += 1;

	if ((_int)ceil(vPos.y) % 2 == 0)
		vPos.y += 1;
	m_pTransform->m_vInfo[INFO_POS].x = (_int)vPos.x;
	m_pTransform->m_vInfo[INFO_POS].y = (_int)vPos.y;
	m_pTransform->m_vInfo[INFO_POS].z = vPos.z;
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