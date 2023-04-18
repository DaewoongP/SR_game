#include "stdafx.h"
#include "UICamera.h"

#include "Export_Function.h"

CUICamera::CUICamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CUICamera::~CUICamera()
{
}

HRESULT CUICamera::Ready_GameObject(void)
{
	D3DXMatrixOrthoLH(&m_matProjection, WINCX, WINCY, 0, 100);

	_vec3 vEye, vAt, vUp;

	vEye = { 0.0f,0.0f,0.0f };
	vAt = { 0.0f,0.0f,1.0f };
	vUp = { 0.0f,1.0f,0.0f };

	D3DXMatrixLookAtLH(&m_matViewSpace, &vEye, &vAt, &vUp);

	return S_OK;
}

_int CUICamera::Update_GameObject(const _float & fTimeDelta)
{
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matViewSpace);
	
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProjection);

	__super::Update_GameObject(fTimeDelta);

	return _int();
}

CUICamera * CUICamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUICamera*		pInstance = new CUICamera(pGraphicDev);

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
