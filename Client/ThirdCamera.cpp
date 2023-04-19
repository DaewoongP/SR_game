#include "stdafx.h"
#include "ThirdCamera.h"


CThirdCamera::CThirdCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}


CThirdCamera::~CThirdCamera()
{
}

HRESULT CThirdCamera::Ready_GameObject(void)
{
	D3DXMatrixIdentity(&m_matRot);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
	m_vViewInfo[EYE] = _vec3(0.f, 24.f, 0.f);
	m_vViewInfo[AT] = _vec3(100.f, 24.f, 9.f);
	m_vViewInfo[UP] = _vec3(0.f, 0.f, -1.f);
	return S_OK;
}

_int CThirdCamera::Update_GameObject(const _float & fTimeDelta)
{
	Update_Matrix();
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	Swap_Camera();
	return 0;
}

void CThirdCamera::Swap_Camera()
{
	if (Engine::Get_DIKeyState(DIK_X) == Engine::KEYDOWN)
		g_Is2D = !g_Is2D;
	CGameObject* pObj = nullptr;
	if (g_Is2D)
	{
		pObj = Engine::Get_GameObject(L"Layer_GameLogic", L"Toodee");
		Set_ToodeeCam(pObj->m_pTransform->m_vInfo[INFO_POS]);
	}
	else
	{
		pObj = Engine::Get_GameObject(L"Layer_GameLogic", L"Topdee");
		Set_TopdeeCam(pObj->m_pTransform->m_vInfo[INFO_POS]);
	}
}

void CThirdCamera::Set_ToodeeCam(_vec3 vPos)
{
	m_vViewInfo[EYE] = vPos + _vec3(-15.f, 0.f, -10.f);
	m_vViewInfo[AT] = vPos;
	m_vViewInfo[UP] = _vec3(0.f, 1.f, 0.f);
}

void CThirdCamera::Set_TopdeeCam(_vec3 vPos)
{
	m_vViewInfo[EYE] = vPos + _vec3(-15.f, 0.f, -8.f);
	m_vViewInfo[AT] = vPos;
	m_vViewInfo[UP] = _vec3(0.f, 0.f, -1.f);
}

void CThirdCamera::Update_Matrix()
{
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(45.f), (_float)WINCX / WINCY, 1.f, 100.f);
	D3DXMatrixLookAtLH(&m_matView, &m_vViewInfo[EYE], &m_vViewInfo[AT], &m_vViewInfo[UP]);
}

CThirdCamera * CThirdCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CThirdCamera*		pInstance = new CThirdCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CThirdCamera::Free(void)
{
	__super::Free();
}
