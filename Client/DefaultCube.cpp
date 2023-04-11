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

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Stage1_Cube", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Stage1_Cube", pComponent });

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
