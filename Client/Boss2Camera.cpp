#include "stdafx.h"
#include "Boss2Camera.h"

CBoss2Camera::CBoss2Camera(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CDirectCamera(pGraphicDev)
{
}

CBoss2Camera::~CBoss2Camera()
{
}

HRESULT CBoss2Camera::Ready_Camera()
{
	m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 60.f,1.0f });
	m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 60.f,1.0f });
	m_vecXYPosAngleSpeed.push_back({ 10.0f ,8.0f, 25.f,1.0f });
	m_vecXYPosAngleSpeed.push_back({ 54.0f ,8.0f, 25.f,0.2f });
	m_vecXYPosAngleSpeed.push_back({ 50.0f ,17.0f, 30.f,5.0f });
	m_vecXYPosAngleSpeed.push_back({ 50.0f ,17.0f, 30.f,0.5f });
	m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 60.f,5.0f });

	return S_OK;
}

CBoss2Camera * CBoss2Camera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoss2Camera*		pInstance = new CBoss2Camera(pGraphicDev);

	if (FAILED(pInstance->Ready_Camera()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss2Camera::Free()
{
	__super::Free();
}
