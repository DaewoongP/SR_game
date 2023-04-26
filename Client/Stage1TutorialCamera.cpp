#include "stdafx.h"
#include "Stage1TutorialCamera.h"

CStage1TutorialCamera::CStage1TutorialCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CDirectCamera(pGraphicDev)
{
}

CStage1TutorialCamera::~CStage1TutorialCamera()
{
}

HRESULT CStage1TutorialCamera::Ready_Camera()
{
	m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 60.f,1.0f });
	m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 60.f,1.0f });
	m_vecXYPosAngleSpeed.push_back({ 4.0f ,30.0f, 20.f,1.0f });
	m_vecXYPosAngleSpeed.push_back({ 4.0f ,30.0f, 20.f,1.0f });
	m_vecXYPosAngleSpeed.push_back({ 56.f ,28.0f, 20.f,1.0f });
	m_vecXYPosAngleSpeed.push_back({ 56.f ,28.0f, 20.f,1.0f });
	m_vecXYPosAngleSpeed.push_back({ 14.0f ,6.0f, 30.f,1.0f });
	m_vecXYPosAngleSpeed.push_back({ 14.0f ,6.0f, 30.f,0.5f });
	m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 60.f,5.0f });

	return S_OK;
}

CStage1TutorialCamera * CStage1TutorialCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage1TutorialCamera*		pInstance = new CStage1TutorialCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Camera()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStage1TutorialCamera::Free()
{
	__super::Free();
}
