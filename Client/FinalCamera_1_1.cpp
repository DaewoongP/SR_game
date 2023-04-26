#include "stdafx.h"
#include "FinalCamera_1_1.h"

CFinalCamera_1_1::CFinalCamera_1_1(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CDirectCamera(pGraphicDev)
{
}

CFinalCamera_1_1::~CFinalCamera_1_1()
{
}

HRESULT CFinalCamera_1_1::Ready_Camera()
{
	//맵
	m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 0.f,1.0f });//0
	m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 0.f,1.0f });//1
	m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 60.f,1.0f });//2
	m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 60.f,1.0f });//3
	m_vecXYPosAngleSpeed.push_back({ 125.0f ,17.0f, 60.f,0.1f });//4
	m_vecXYPosAngleSpeed.push_back({ 125.0f ,17.0f, 60.f,1.0f });//5
	//보스
	m_vecXYPosAngleSpeed.push_back({ 100.f ,-50.f, 120.f,60.0f });//6
	m_vecXYPosAngleSpeed.push_back({ 150.f ,50.f, 120.f,1.0f });//7
	m_vecXYPosAngleSpeed.push_back({ 150.f ,-50.f, 120.f,0.1f });//8

	return S_OK;
}

_int CFinalCamera_1_1::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	switch (m_iIndex)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
		Update_Direct(fTimeDelta);
		break;
	case 6:
	case 7:
	case 8:
		Update_Direct_X(fTimeDelta);
		break;
	default:
		break;
	}

	__super::Update_GameObject(fTimeDelta);
	//렌더러에 넣어주기
	Engine::Add_RenderGroup(RENDER_ALPHABLEND, this);

	return 0;
}

void CFinalCamera_1_1::Update_Direct_X(const _float & fTimeDelta)
{
	m_fTimer += fTimeDelta * m_vecXYPosAngleSpeed[m_iIndex].w;

	_vec4 pOut;

	if (m_fTimer >= 1.0f)
	{
		m_fTimer = 1.0f;
	}

	D3DXVec4Lerp(&pOut, &m_vecXYPosAngleSpeed[m_iIndex - 1], &m_vecXYPosAngleSpeed[m_iIndex], m_fTimer);

	_vec3 vShake = { 0.0f,0.0f, 10.f };

	m_pTransform->Update_Shake(fTimeDelta, vShake);

	m_vCam[EYE].x = pOut.x + vShake.x;
	m_vCam[EYE].y = pOut.y + vShake.y;
	m_vCam[EYE].z = vShake.z;
	m_vCam[AT].x = m_vCam[EYE].x +1.0f;
	m_vCam[AT].y = m_vCam[EYE].y;
	m_vCam[AT].z = m_vCam[EYE].z;

	m_vCam[UP] = { 0.0f,0.0f,-1.0f };

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

CFinalCamera_1_1 * CFinalCamera_1_1::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFinalCamera_1_1*		pInstance = new CFinalCamera_1_1(pGraphicDev);

	if (FAILED(pInstance->Ready_Camera()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFinalCamera_1_1::Free()
{
	__super::Free();
}
