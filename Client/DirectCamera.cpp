#include "stdafx.h"
#include "DirectCamera.h"

#include "Export_Function.h"


CDirectCamera::CDirectCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCamera(pGraphicDev), m_iIndex(1), m_fTimer(0.0f)
{
}

CDirectCamera::~CDirectCamera()
{
}

_int CDirectCamera::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Direct(fTimeDelta);

	CCamera::Update_GameObject(fTimeDelta);

	return 0;
}

void CDirectCamera::Update_Direct(const _float& fTimeDelta)
{
	m_fTimer += fTimeDelta * m_vecXYPosAngleSpeed[m_iIndex].w;

	_vec4 pOut;

	if (m_fTimer >= 1.0f)
	{
		m_fTimer = 1.0f;
	}

	D3DXVec4Lerp(&pOut, &m_vecXYPosAngleSpeed[m_iIndex-1], &m_vecXYPosAngleSpeed[m_iIndex], m_fTimer);
	
	_vec3 vShake = { 0.0f,0.0f,-22.0f };

	m_pTransform->Update_Shake(fTimeDelta, vShake);

	m_vCam[EYE].x = pOut.x+vShake.x;
	m_vCam[EYE].y = pOut.y+vShake.y;
	m_vCam[EYE].z = vShake.z;
	m_vCam[AT].x = pOut.x;
	m_vCam[AT].y = pOut.y;

	m_fProj[FOV] = D3DXToRadian(pOut.z);

	if (1.0f == m_fTimer)
	{
		++m_iIndex;
		m_fTimer = 0.0f;
		if (m_iIndex == m_vecXYPosAngleSpeed.size())
		{
			m_bDead = true;
		}
	}
}


void CDirectCamera::Free()
{
	__super::Free();
}
