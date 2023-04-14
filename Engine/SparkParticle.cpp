#include "stdafx.h"
#include "SparkParticle.h"

#include "Export_Function.h"
CSparkParticle::CSparkParticle(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath, _int iTextureNum, _float fSize, _int iParticleNum, _bool isWorld)
	:CParticleSystem(pGraphicDev)
{
	m_pTexture = CTexture::Create(m_pGraphicDev,
		TEX_NORMAL,
		pPath,
		iTextureNum);
	m_pTexture->Add_Anim(L"Idle", 0, iTextureNum - 1, 20.f, true);
	m_pTexture->Switch_Anim(L"Idle");
	m_pTexture->m_bUseFrameAnimation = true;
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

CSparkParticle::CSparkParticle(const CSparkParticle & rhs)
	:CParticleSystem(rhs),
	m_fLifeTime(rhs.m_fLifeTime)
{
	for (auto& iter : rhs.m_Particles)
		m_Particles.push_back(iter);
}

CSparkParticle::~CSparkParticle()
{
}

void CSparkParticle::ResetParticle(Particle * particle)
{
	particle->bIsAlive = true;
	particle->dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	particle->vVelocity = { 0.f, 0.f, 0.f };
	GetRandomVector(
		&particle->vPos,
		&m_BoundingBox._min,
		&m_BoundingBox._max);
	particle->fAge = 0.f;
	particle->fLifeTime = m_fLifeTime;
}

_int CSparkParticle::Update_Particle()
{
	if (!m_bTrigger)
		return 0;
	__super::Update_Particle();
	_float fTimeDelta = Engine::Get_Timer(L"Timer_FPS60");
	for (auto& it = m_Particles.begin(); it != m_Particles.end(); it++)
	{
		if (it->bIsAlive)
		{
			if (0 == it->fLifeTime)
				break;
			m_Size *= 0.995f;
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

CSparkParticle * CSparkParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath, _int iTextureNum, _float fSize, _int iParticleNum, _bool isWorld)
{
	CSparkParticle *	pInstance = new CSparkParticle(pGraphicDev, pPath, iTextureNum, fSize, iParticleNum, isWorld);

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CSparkParticle::Clone(void)
{
	return new CSparkParticle(*this);
}

void CSparkParticle::Free(void)
{
	__super::Free();
}
