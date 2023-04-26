#include "stdafx.h"
#include "ShootingCamera.h"


CShootingCamera::CShootingCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCamera(pGraphicDev)
{
}


CShootingCamera::~CShootingCamera()
{
}

HRESULT CShootingCamera::Ready_Camera()
{
	m_fProj[FOV] = D3DXToRadian(45.f);
	m_fProj[ASPECT] = (_float)WINCX / WINCY;
	m_fProj[NEAR_] = 1.f;
	m_fProj[FAR_] = 500.f;
	m_vCam[EYE] = _vec3(-20.f, 20.f, 0.f);
	m_vCam[AT] = _vec3(280.f, 20.f, 0.f);
	m_vCam[UP] = _vec3(0.f, 0.f, -1.f);
	return S_OK;
}

_int CShootingCamera::Update_GameObject(const _float & fTimeDelta)
{
	
	__super::Update_GameObject(fTimeDelta);
	return 0;
}

void CShootingCamera::Render_GameObject()
{



	__super::Render_GameObject();
}

CShootingCamera * CShootingCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShootingCamera*		pInstance = new CShootingCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Camera()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CShootingCamera::Free()
{
	__super::Free();
}
