#include "stdafx.h"
#include "MoveCamera.h"


CMoveCamera::CMoveCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}


CMoveCamera::~CMoveCamera()
{
}

HRESULT CMoveCamera::Ready_GameObject(void)
{
	D3DXMatrixIdentity(&m_matRot);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
	m_vViewInfo[EYE] = _vec3(3.f, 24.f, 0.f);
	m_vViewInfo[AT] = _vec3(20.f, 24.f, 10.f);
	D3DXVec3Cross(&m_vViewInfo[UP], &_vec3(0.f, 1.f, 0.f), &(m_vViewInfo[AT] - m_vViewInfo[EYE]));
	m_fSpeed = 50.f;
	return S_OK;
}

_int CMoveCamera::Update_GameObject(const _float & fTimeDelta)
{
	Move_Cam(fTimeDelta);



	Update_Matrix();
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
	return 0;
}

void CMoveCamera::Update_Matrix()
{
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(45.f), (_float)WINCX / WINCY, 1.f, 500.f);
	D3DXMatrixLookAtLH(&m_matView, &m_vViewInfo[EYE], &m_vViewInfo[AT], &m_vViewInfo[UP]);
}

void CMoveCamera::Move_Cam(const _float& fTimeDelta)
{
	if (Engine::Get_DIKeyState(DIK_UP) == Engine::KEYPRESS)
	{
		m_vViewInfo[EYE].x += m_fSpeed * fTimeDelta;
		m_vViewInfo[AT].x += m_fSpeed * fTimeDelta;
	}
	if (Engine::Get_DIKeyState(DIK_DOWN) == Engine::KEYPRESS)
	{
		m_vViewInfo[EYE].x -= m_fSpeed * fTimeDelta;
		m_vViewInfo[AT].x -= m_fSpeed * fTimeDelta;
	}

	if (Engine::Get_DIKeyState(DIK_RIGHT) == Engine::KEYPRESS)
	{
		m_vViewInfo[EYE].y -= m_fSpeed * fTimeDelta;
		m_vViewInfo[AT].y -= m_fSpeed * fTimeDelta;
	}
	if (Engine::Get_DIKeyState(DIK_LEFT) == Engine::KEYPRESS)
	{
		m_vViewInfo[EYE].y += m_fSpeed * fTimeDelta;
		m_vViewInfo[AT].y += m_fSpeed * fTimeDelta;
	}

	if (Engine::Get_DIKeyState(DIK_Z) == Engine::KEYPRESS)
	{
		m_vViewInfo[EYE].z += m_fSpeed * fTimeDelta;
		m_vViewInfo[AT].z += m_fSpeed * fTimeDelta;
	}

	if (Engine::Get_DIKeyState(DIK_X) == Engine::KEYPRESS)
	{
		m_vViewInfo[EYE].z -= m_fSpeed * fTimeDelta;
		m_vViewInfo[AT].z -= m_fSpeed * fTimeDelta;
	}
}

CMoveCamera * CMoveCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMoveCamera*		pInstance = new CMoveCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMoveCamera::Free(void)
{
	__super::Free();
}
