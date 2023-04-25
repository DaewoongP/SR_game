#include "stdafx.h"
#include "Boss2Camera.h"

#include "Export_Function.h"


CBoss2Camera::CBoss2Camera(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCamera(pGraphicDev), m_iIndex(0), m_fTimer(0.0f)
{
}

CBoss2Camera::~CBoss2Camera()
{
}

HRESULT CBoss2Camera::Ready_Camera()
{
	m_iIndex = 1;
	m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 60.f,1.0f });
	m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 60.f,1.0f });
	m_vecXYPosAngleSpeed.push_back({ 10.0f ,10.0f, 30.f,1.0f });
	m_vecXYPosAngleSpeed.push_back({ 10.0f ,10.0f, 30.f,1.0f });
	m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 60.f,1.0f });

	return S_OK;
}

_int CBoss2Camera::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	Update_Direct(fTimeDelta);

	__super::Update_GameObject(fTimeDelta);
	//렌더러에 넣어주기
	Engine::Add_RenderGroup(RENDER_ALPHABLEND, this);

	return 0;
}

void CBoss2Camera::Update_Direct(const _float& fTimeDelta)
{
	m_fTimer += fTimeDelta * m_vecXYPosAngleSpeed[m_iIndex].w;

	_vec4 pOut;

	if (m_fTimer >= 1.0f)
	{
		m_fTimer = 1.0f;
	}

	D3DXVec4Lerp(&pOut, &m_vecXYPosAngleSpeed[m_iIndex-1], &m_vecXYPosAngleSpeed[m_iIndex], m_fTimer);
	m_vCam[EYE].x = pOut.x;
	m_vCam[EYE].y = pOut.y;
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
