#include "stdafx.h"
#include "ProduceObject.h"

CProduceObject::CProduceObject(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev), m_fProdTimer(0.0f), m_iIndex(1), m_bStart(false), m_bNext(false), m_eID(LOADING_END)
{
}

CProduceObject::~CProduceObject()
{
}

void CProduceObject::Update_Produce(const _float & fTimeDelta)
{
	if (m_iIndex >= m_vecLerpList.size())
	{
		return;
	}

	m_fProdTimer += fTimeDelta * m_vecLerpList[m_iIndex].z;

	_vec3 pOut;

	if (m_fProdTimer >= 1.0f)
	{
		m_fProdTimer = 1.0f;
	}

	D3DXVec3Lerp(&pOut, &m_vecLerpList[m_iIndex - 1], &m_vecLerpList[m_iIndex], m_fProdTimer);

	_vec3 vShake = { 0.0f,0.0f,0.0f };

	m_pTransform->Update_Shake(fTimeDelta, vShake);

	m_pTransform->m_vInfo[INFO_POS].x = pOut.x + vShake.x;
	m_pTransform->m_vInfo[INFO_POS].y = pOut.y + vShake.y;

	
}

void CProduceObject::On_Next(int iIndex)
{
	if (1.0f == m_fProdTimer)
	{
		++m_iIndex;
		m_fProdTimer = 0.0f;
		if (m_iIndex == iIndex)
		{
			m_bNext = true;
		}
	}
}

void CProduceObject::Free(void)
{
	__super::Free();
}
