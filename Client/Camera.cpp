#include "stdafx.h"
#include "Camera.h"


CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);

	m_vCam[EYE] = { 31.0f ,17.0f,-22.0f };
	m_vCam[AT] = { 31.0f ,17.0f,0.0f };
	m_vCam[UP] = { 0.0f, 1.0f, 0.0f };

	m_fProj[FOV] = D3DXToRadian(60.f);
	m_fProj[ASPECT] = (_float)WINCX / WINCY;
	m_fProj[NEAR_] = 1.f;
	m_fProj[FAR_] = 100.f;

	m_fSpeed = 10.f;
}


CCamera::~CCamera()
{
}

_int CCamera::Update_GameObject(const _float & fTimeDelta)
{
	Add_RenderGroup(RENDER_NONE, this);
	__super::Update_GameObject(fTimeDelta);
	return 0;
}

void CCamera::Render_GameObject()
{
	Update_Matrix();
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);
	__super::Render_GameObject();
}

void CCamera::Update_Matrix()
{
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
	D3DXMatrixLookAtLH(&m_matView, &m_vCam[EYE], &m_vCam[AT], &m_vCam[UP]);
	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fProj[FOV], m_fProj[ASPECT], m_fProj[NEAR_], m_fProj[FAR_]);
}

void CCamera::Free()
{
	__super::Free();
}
