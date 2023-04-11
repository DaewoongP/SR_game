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
	D3DXMatrixOrthoLH(&matProj, WINCX, WINCY, 1, 0);

	_vec3 vEye, vAt, vUp;

	vEye = { 0.0f,0.0f,-1.0f };
	vAt = { 0.0f,0.0f,0.0f };
	vUp = { 0.0f,1.0f,0.0f };

	D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
	return S_OK;
}

_int CUICamera::Update_GameObject(const _float& fTimeDelta)
{
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);

	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
	__super::Update_GameObject(fTimeDelta);


	return 0;
}

void CUICamera::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
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
