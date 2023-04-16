#include "stdafx.h"
#include "LogoCamera.h"

#include "Export_Function.h"

CLogoCamera::CLogoCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CLogoCamera::~CLogoCamera()
{
}

HRESULT CLogoCamera::Ready_GameObject(void)
{
	D3DXMatrixPerspectiveFovLH(&m_matProjection, D3DXToRadian(90), (_float)WINCX / WINCY, 1.0f, 1000.0f);

	_vec3 vEye, vAt, vUp;

	vEye = { 0.0f,0.0f,-2.0f };
	vAt = { 0.0f,0.0f,0.0f };
	vUp = { 0.0f,1.0f,0.0f };

	D3DXMatrixLookAtLH(&m_matViewSpace, &vEye, &vAt, &vUp);

	return S_OK;
}

_int CLogoCamera::Update_GameObject(const _float & fTimeDelta)
{
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matViewSpace);

	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProjection);

	__super::Update_GameObject(fTimeDelta);

	return _int();
}

CLogoCamera * CLogoCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLogoCamera*		pInstance = new CLogoCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CLogoCamera::Free()
{
	__super::Free();
}