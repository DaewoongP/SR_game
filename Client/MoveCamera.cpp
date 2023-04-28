#include "stdafx.h"
#include "MoveCamera.h"


CMoveCamera::CMoveCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCamera(pGraphicDev)
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
	m_vCam[EYE] = _vec3(3.f, 24.f, 0.f);
	m_vCam[AT] = _vec3(20.f, 24.f, 10.f);
	D3DXVec3Cross(&m_vCam[UP], &_vec3(0.f, 1.f, 0.f), &(m_vCam[AT] - m_vCam[EYE]));
	m_fProj[FOV] = D3DXToRadian(45.f);
	m_fProj[FAR_] = 500.f;
	m_fSpeed = 50.f;
	return S_OK;
}

_int CMoveCamera::Update_GameObject(const _float & fTimeDelta)
{
	Move_Cam(fTimeDelta);
	__super::Update_GameObject(fTimeDelta);
	return 0;
}

void CMoveCamera::Render_GameObject()
{
	__super::Render_GameObject();
}

void CMoveCamera::Move_Cam(const _float& fTimeDelta)
{
	if (Engine::Get_DIKeyState(DIK_UP) == Engine::KEYPRESS)
	{
		m_vCam[EYE].x += m_fSpeed * fTimeDelta;
		m_vCam[AT].x += m_fSpeed * fTimeDelta;
	}
	if (Engine::Get_DIKeyState(DIK_DOWN) == Engine::KEYPRESS)
	{
		m_vCam[EYE].x -= m_fSpeed * fTimeDelta;
		m_vCam[AT].x -= m_fSpeed * fTimeDelta;
	}

	if (Engine::Get_DIKeyState(DIK_RIGHT) == Engine::KEYPRESS)
	{
		m_vCam[EYE].y -= m_fSpeed * fTimeDelta;
		m_vCam[AT].y -= m_fSpeed * fTimeDelta;
	}
	if (Engine::Get_DIKeyState(DIK_LEFT) == Engine::KEYPRESS)
	{
		m_vCam[EYE].y += m_fSpeed * fTimeDelta;
		m_vCam[AT].y += m_fSpeed * fTimeDelta;
	}

	if (Engine::Get_DIKeyState(DIK_Z) == Engine::KEYPRESS)
	{
		m_vCam[EYE].z += m_fSpeed * fTimeDelta;
		m_vCam[AT].z += m_fSpeed * fTimeDelta;
	}

	if (Engine::Get_DIKeyState(DIK_X) == Engine::KEYPRESS)
	{
		m_vCam[EYE].z -= m_fSpeed * fTimeDelta;
		m_vCam[AT].z -= m_fSpeed * fTimeDelta;
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
