#include "stdafx.h"
#include "CircularParticle.h"

#include "Export_Function.h"
CCircularParticle::CCircularParticle(LPDIRECT3DDEVICE9 pGraphicDev,
	const _tchar * pPath,
	_int iTextureNum, 
	_float fSize, 
	_int iParticleNum, 
	_bool isWorld,
	_float fLifeTime,
	_float fRadius)
	:CParticleSystem(pGraphicDev)
{
	m_fLifeTime = fLifeTime;
	m_fRadius = fRadius;
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
	m_bSetRandomGenTime = false;
	m_fSizeoverLifetime = 0.995f;
	m_fRand = 3.f;
	m_dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	for (int i = 0; i < iParticleNum; i++)
		AddParticle();
}

CCircularParticle::CCircularParticle(const CCircularParticle & rhs)
	:CParticleSystem(rhs),
	m_fLifeTime(rhs.m_fLifeTime),
	m_fRadius(rhs.m_fRadius),
	m_bSetRandomGenTime(rhs.m_bSetRandomGenTime),
	m_dwColor(rhs.m_dwColor),
	m_fSizeoverLifetime(rhs.m_fSizeoverLifetime),
	m_fRand(rhs.m_fRand)
{
	for (auto& iter : rhs.m_Particles)
		m_Particles.push_back(iter);
}

CCircularParticle::~CCircularParticle()
{
}

void CCircularParticle::ResetParticle(Particle * particle)
{
	particle->bIsAlive = true;
	particle->dwColor = m_dwColor;
	particle->vPos = m_BoundingBox.Get_Center();
	particle->vVelocity = { 1.f, 0.f, 0.f };
	GetRandomVectorIncircle(&particle->vVelocity, m_fRadius);
	if (m_bSetRandomGenTime)
		particle->fGenTime = GetRandomFloat(0.f, m_fRand);
	particle->fAge = 0.f;
	particle->fLifeTime = m_fLifeTime + particle->fGenTime;
	particle->fSizeoverLifetime = m_fSizeoverLifetime;
	D3DXVec3Normalize(&particle->vAccel, &particle->vVelocity);
	particle->vAccel *= -0.5;
}

_int CCircularParticle::Update_Particle()
{
	if (!m_bTrigger)
		return 0;
	__super::Update_Particle();
	_float fTimeDelta = Engine::Get_Timer(L"Timer_FPS60");

	for (auto& it = m_Particles.begin(); it != m_Particles.end(); it++)
	{
		if (it->bIsAlive)
		{
			it->fAge += fTimeDelta;
			if (it->fGenTime != 0 && it->fGenTime > it->fAge)
				continue;

			it->vPos += it->vVelocity * fTimeDelta;
			it->vVelocity += it->vAccel;
			if (D3DXVec3Length(&it->vAccel) > D3DXVec3Length(&it->vVelocity))
				it->vAccel = { 0.f, 0.f, 0.f };
			m_Size *= it->fSizeoverLifetime;

			if (it->fAge > it->fLifeTime)
				it->bIsAlive = false;
		}
	}
	__super::Render_Particle();
	return -1;
}

CCircularParticle * CCircularParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev,
	const _tchar * pPath,
	_int iTextureNum,
	_float fSize,
	_int iParticleNum,
	_bool isWorld,
	_float fLifeTime,
	_float fRadius)
{
	CCircularParticle *	pInstance = new CCircularParticle(pGraphicDev, 
		pPath,
		iTextureNum, 
		fSize,
		iParticleNum, 
		isWorld,
		fLifeTime,
		fRadius);

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CCircularParticle::Clone(void)
{
	return new CCircularParticle(*this);
}

void CCircularParticle::Free(void)
{
	__super::Free();
}
