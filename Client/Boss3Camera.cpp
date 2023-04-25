#include "stdafx.h"
#include "Boss3Camera.h"

CBoss3Camera::CBoss3Camera(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CDirectCamera(pGraphicDev)
{
}

CBoss3Camera::~CBoss3Camera()
{
}

HRESULT CBoss3Camera::Ready_Camera()
{
	m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 60.f,1.0f });//1
	m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 60.f,1.0f });//1
	m_vecXYPosAngleSpeed.push_back({ 6.0f ,30.0f, 20.f,1.0f });//1
	m_vecXYPosAngleSpeed.push_back({ 6.0f ,30.0f, 20.f,10.0f });//0.1
	m_vecXYPosAngleSpeed.push_back({ 58.0f ,30.0f, 20.f,0.2f });//5
	m_vecXYPosAngleSpeed.push_back({ 58.0f ,30.0f, 20.f,10.0f });//0.1
	m_vecXYPosAngleSpeed.push_back({ 60.0f ,18.0f, 20.f,5.0f });//0.2
	m_vecXYPosAngleSpeed.push_back({ 60.0f ,18.0f, 20.f,0.5f });//2
	m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 45.f,10.0f });//0.1
	m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 45.f,0.5f });
	m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 60.f,5.0f });

	return S_OK;
}

CBoss3Camera * CBoss3Camera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoss3Camera*		pInstance = new CBoss3Camera(pGraphicDev);

	if (FAILED(pInstance->Ready_Camera()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss3Camera::Free()
{
	__super::Free();
}