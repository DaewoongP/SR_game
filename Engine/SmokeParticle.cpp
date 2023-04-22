#include "stdafx.h"
#include "SmokeParticle.h"

#include "Export_Function.h"
CSmokeParticle::CSmokeParticle(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath, _int iTextureNum, _float fSize, _int iParticleNum, _bool isWorld)
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
	for (int i = 0; i < iParticleNum; i++)
		AddParticle();
}

CSmokeParticle::CSmokeParticle(const CSmokeParticle & rhs)
	:CParticleSystem(rhs)
{
	m_fGenTime = 0.f;
	for (auto& iter : rhs.m_Particles)
		m_Particles.push_back(iter);
	m_fParticleNum = m_Particles.size();
}

CSmokeParticle::~CSmokeParticle()
{
}

void CSmokeParticle::ResetParticle(Particle * particle)
{
	particle->bIsAlive = true;
	particle->dwColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	particle->vVelocity = _vec3(0.f, 1.f, 0.f);
	particle->vPos = m_BoundingBox.Get_Center();
	particle->vPos.x += GetRandomFloat(-0.3f, 0.3f);
	particle->vPos.z += 0.1f;
	particle->fGenTime = m_fGenTime;
	particle->fAge = 0.f;
	particle->fLifeTime = m_fParticleNum * 0.5f + m_fGenTime;
	m_fGenTime += 0.5f;
}

_int CSmokeParticle::Update_Particle()
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
			if (it->fAge > it->fLifeTime)
				it->bIsAlive = false;
		}
		else
		{
			_float fAge = it->fAge;
			ResetParticle(&*it);
			it->fAge = fAge;
		}
	}
	__super::Render_Particle();
	return -1;
}

CSmokeParticle * CSmokeParticle::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar * pPath, _int iTextureNum, _float fSize, _int iParticleNum, _bool isWorld)
{
	CSmokeParticle *	pInstance = new CSmokeParticle(pGraphicDev, pPath, iTextureNum, fSize, iParticleNum, isWorld);

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CSmokeParticle::Clone(void)
{
	return new CSmokeParticle(*this);
}

void CSmokeParticle::Free(void)
{
	__super::Free();
}
