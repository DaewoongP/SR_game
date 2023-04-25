#include "stdafx.h"
#include "SuperLandingParticle.h"

#include "Export_Function.h"
CSuperLandingParticle::CSuperLandingParticle(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath, _int iTextureNum, _float fSize, _int iParticleNum, _bool isWorld)
	:CParticleSystem(pGraphicDev)
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
	m_isOdd = true;
	for (int i = 0; i < iParticleNum; i++)
		AddParticle();
}

CSuperLandingParticle::CSuperLandingParticle(const CSuperLandingParticle & rhs)
	:CParticleSystem(rhs),
	m_isOdd(rhs.m_isOdd)
{
	for (auto& iter : rhs.m_Particles)
		m_Particles.push_back(iter);
}

CSuperLandingParticle::~CSuperLandingParticle()
{
}

void CSuperLandingParticle::ResetParticle(Particle * particle)
{
	particle->bIsAlive = true;
	particle->dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	particle->vPos = m_BoundingBox.Get_Center();
	particle->vStart = particle->vPos;
	particle->vEnd = particle->vPos + _vec3(30.f, GetRandomFloat(0.f, 10.f), GetRandomFloat(-3.f, 2.f));
	particle->fRandFloat = 0.f;
	if (m_isOdd)
	{
		particle->vEnd.x = particle->vStart.x - 30.f;
		m_isOdd = !m_isOdd;
	}
	else
		m_isOdd = !m_isOdd;
}

_int CSuperLandingParticle::Update_Particle()
{
	if (!m_bTrigger)
		return 0;
	__super::Update_Particle();

	_float fTimeDelta = Engine::Get_Timer(L"Timer_FPS60");
	
	for (auto& it = m_Particles.begin(); it != m_Particles.end(); it++)
	{
		if (it->bIsAlive)
		{
			it->fRandFloat += GetRandomFloat(0.01f, 0.1f) * 0.25f;
			if (it->vEnd.x > it->vStart.x)
				GetVectorSlerp(&it->vPos, &it->vStart, &it->vEnd, &_vec3(0, 0, 1), 30.f, it->fRandFloat);
			else
				GetVectorSlerp(&it->vPos, &it->vStart, &it->vEnd, &_vec3(0, 0, -1), 30.f, it->fRandFloat);
			if (it->fRandFloat >= 1.f)
			{
				it->bIsAlive = false;
				End_Particle();
			}
		}
	}
	__super::Render_Particle();
	return -1;
}

CSuperLandingParticle * CSuperLandingParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath, _int iTextureNum, _float fSize, _int iParticleNum, _bool isWorld)
{
	CSuperLandingParticle *	pInstance = new CSuperLandingParticle(pGraphicDev, pPath, iTextureNum, fSize, iParticleNum, isWorld);

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CSuperLandingParticle::Clone(void)
{
	return new CSuperLandingParticle(*this);
}

void CSuperLandingParticle::Free(void)
{
	__super::Free();
}
