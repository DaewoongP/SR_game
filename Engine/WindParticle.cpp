#include "stdafx.h"
#include "WindParticle.h"

#include "Export_Function.h"
CWindParticle::CWindParticle(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath, _int iTextureNum, _float fSize, _int iParticleNum, _bool isWorld)
	:CParticleSystem(pGraphicDev)
{
	m_pTexture = CTexture::Create(m_pGraphicDev,
		TEX_NORMAL,
		pPath);
	// 객체의 Ready에서도 변경 가능
	m_Size = fSize;
	// 적절하게 크기값 조절하면됨.
	m_VBSize = 8;
	m_VBOffset = 0;
	m_VBBatchSize = 8;

	for (int i = 0; i < iParticleNum; i++)
		AddParticle();
}

CWindParticle::CWindParticle(const CWindParticle & rhs)
	:CParticleSystem(rhs),
	m_fLifeTime(rhs.m_fLifeTime)
{
	for (auto& iter : rhs.m_Particles)
		m_Particles.push_back(iter);
}

CWindParticle::~CWindParticle()
{
}

void CWindParticle::ResetParticle(Particle * particle)
{
	particle->bIsAlive = true;
	particle->dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 0.5f);
	particle->vVelocity = { 20.f + rand() % 30, _float(rand() % 3), 0.f };
	GetRandomVector(
		&particle->vPos,
		&m_BoundingBox._min,
		&m_BoundingBox._max);
	particle->vPos.x = m_BoundingBox._min.x;
}

_int CWindParticle::Update_Particle()
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
				ResetParticle(&(*it));
			}

			it->fAge += fTimeDelta;
			if (it->fAge > it->fLifeTime)
				it->bIsAlive = false;
		}
		else
			ResetParticle(&(*it));
	}
	__super::Render_Particle();
	return -1;
}

CWindParticle * CWindParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath, _int iTextureNum, _float fSize, _int iParticleNum, _bool isWorld)
{
	CWindParticle *	pInstance = new CWindParticle(pGraphicDev, pPath, iTextureNum, fSize, iParticleNum, isWorld);

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CWindParticle::Clone(void)
{
	return new CWindParticle(*this);
}

void CWindParticle::Free(void)
{
	__super::Free();
}
