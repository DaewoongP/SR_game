#include "stdafx.h"
#include "WaterParticle.h"

#include "Export_Function.h"
CWaterParticle::CWaterParticle(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, _int iTextureNum, _float fSize, _int iParticleNum, _bool isWorld)
	:CParticleSystem(pGraphicDev),
	m_iRandOffset(8)
{
	m_pTexture = CTexture::Create(m_pGraphicDev,
		TEX_NORMAL,
		pPath);
	m_bIsWorld = isWorld;
	// 객체의 Ready에서도 변경 가능
	m_Size = fSize;
	// 적절하게 크기값 조절하면됨.
	m_VBSize = 8;
	m_VBOffset = 0;
	m_VBBatchSize = 8;

	for (int i = 0; i < iParticleNum; i++)
		AddParticle();
}

CWaterParticle::CWaterParticle(const CWaterParticle& rhs)
	:CParticleSystem(rhs),
	m_iRandOffset(rhs.m_iRandOffset)
{
	for (auto& iter : rhs.m_Particles)
		m_Particles.push_back(iter);
}

CWaterParticle::~CWaterParticle()
{
}

void CWaterParticle::ResetParticle(Particle* particle)
{
	particle->bIsAlive = true;
	particle->dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	particle->vVelocity = { 0.f, -30.f, 0.f };
	GetRandomVector(&particle->vPos,
		&m_BoundingBox._min,
		&m_BoundingBox._max);
	particle->vPos.y = m_BoundingBox._max.y - (rand() % m_iRandOffset);
}

_int CWaterParticle::Update_Particle()
{
	if (!m_bTrigger)
		return 0;
	__super::Update_Particle();
	_float fTimeDelta = Engine::Get_Timer(L"Timer_FPS60");
	for (auto& it = m_Particles.begin(); it != m_Particles.end(); it++)
	{
		if (it->bIsAlive)
		{
			it->vPos += it->vVelocity * fTimeDelta;
			if (m_BoundingBox.Intersect(it->vPos) == false)
			{
				it->bIsAlive = false;
			}
		}
		else
			ResetParticle(&(*it));
	}
	__super::Render_Particle();
	return -1;
}

CWaterParticle* CWaterParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, _int iTextureNum, _float fSize, _int iParticleNum, _bool isWorld)
{
	CWaterParticle* pInstance = new CWaterParticle(pGraphicDev, pPath, iTextureNum, fSize, iParticleNum, isWorld);

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent* CWaterParticle::Clone(void)
{
	return new CWaterParticle(*this);
}

void CWaterParticle::Free(void)
{
	__super::Free();
}
