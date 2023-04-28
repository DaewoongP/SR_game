#include "stdafx.h"
#include "DefaultCube.h"

#include "Export_Function.h"
CDefaultCube::CDefaultCube(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCube(pGraphicDev),
	m_iCubeIndex(0)
{
}

CDefaultCube::~CDefaultCube()
{
}

HRESULT CDefaultCube::Ready_GameObject(_vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };
	return S_OK;
}

_int CDefaultCube::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	On_Grid();

	CGameObject::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);

	return 0;
}

void CDefaultCube::LateUpdate_GameObject(void)
{
	CGameObject::LateUpdate_GameObject();
}

void CDefaultCube::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(m_iCubeIndex);

	m_pBufferCom->Render_Buffer();

	CGameObject::Render_GameObject();
}

void CDefaultCube::OnCollisionEnter(const Collision * collision)
{
	CGameObject::OnCollisionEnter(collision);
}

void CDefaultCube::OnCollisionStay(const Collision * collision)
{
	CGameObject::OnCollisionStay(collision);
}

void CDefaultCube::OnCollisionExit(const Collision * collision)
{
	CGameObject::OnCollisionExit(collision);
}

HRESULT CDefaultCube::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"CubeTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"CubeTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Stage_Cube", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Stage_Cube", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });
	m_pCollider->Set_BoundingBox({ 2.f,2.f,2.f });
	m_pCollider->Set_Group(COL_ENV);

	return S_OK;
}

void CDefaultCube::On_Grid()
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

CDefaultCube * CDefaultCube::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos)
{
	CDefaultCube*		pInstance = new CDefaultCube(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CDefaultCube::Free(void)
{
	CGameObject::Free();
}
