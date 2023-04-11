#include "stdafx.h"
#include "UICamera.h"
#include"..\Engine\Export_Function.h"
CUICamera::CUICamera(LPDIRECT3DDEVICE9 pGraphicDev):CGameObject(pGraphicDev)
{
}

CUICamera::~CUICamera()
{
}

HRESULT CUICamera::Ready_GameObject(void)
{
	Rotation_View();
	_matrix matProj;
	D3DXMatrixOrthoLH(&matProj, 300.f, 200.f, 1.f, 1000.f);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, D3DXMatrixIdentity(&matProj));
	//m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

	return S_OK;
}

_int CUICamera::Update_GameObject(const _float& fTimeDelta)
{
	Rotation_View();
	__super::Update_GameObject(fTimeDelta);


	return 0;
}

void CUICamera::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CUICamera::Rotation_View()
{
	_float fRadian;

	fRadian = m_pTransform->m_vAngle.x;

	vEye = { 31.0f ,17.0f,-22.0f };
	vAt = { 31.0f ,17.0f,0.0f };

	D3DXMatrixIdentity(&matRotX);

	vAt.y -= sinf(D3DXToRadian(fRadian)) * 9.0f;
	vEye.y -= sinf(D3DXToRadian(fRadian)) * 22.0f;
	vEye.z -= (1 - cosf(D3DXToRadian(fRadian))) * 22.0f;

	_vec3 vDir;
	vDir = vEye - vAt;
	D3DXVec3TransformCoord(&vDir, &vDir, &matRotX);

	vEye = vAt + vDir;

	_vec3 vAxisX = { 1.f, 0.f, 0.f };
	// 카메라의 업벡터
	D3DXVec3Cross(&vUp, &(-vDir), &vAxisX);

	_matrix viewMatrix;
	D3DXMatrixLookAtLH(&viewMatrix, &vEye, &vAt, &vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &viewMatrix);
}
void CUICamera::Move_Camera(const _float& fTimeDelta)
{
}

CUICamera* CUICamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUICamera* pInstance = new CUICamera(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CUICamera::Free()
{
	__super::Free();
}
