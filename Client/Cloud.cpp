#include "stdafx.h"
#include "Cloud.h"



CCloud::CCloud(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CCloud::~CCloud()
{
}

_int CCloud::Update_Too(const _float & fTimeDelta)
{
	if (CLOUDTOOZ > m_pTransform->m_vInfo[INFO_POS].z)
	{
		m_pTransform->m_vInfo[INFO_POS].z += (CLOUDTOOZ - CLOUDTOPZ) * fTimeDelta * 4.0f;
	}
	else
		m_pTransform->m_vInfo[INFO_POS].z = CLOUDTOOZ;

	__super::Update_Too(fTimeDelta);
	return S_OK;
}

_int CCloud::Update_Top(const _float & fTimeDelta)
{
	if (CLOUDTOPZ < m_pTransform->m_vInfo[INFO_POS].z)
	{
		m_pTransform->m_vInfo[INFO_POS].z -= (CLOUDTOOZ - CLOUDTOPZ) * fTimeDelta * 4.0f;
	}
	else
		m_pTransform->m_vInfo[INFO_POS].z = CLOUDTOPZ;
	__super::Update_Top(fTimeDelta);
	return S_OK;
}
