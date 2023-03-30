#include "stdafx.h"
#include "Firework.h"




CFirework::CFirework(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 * origin, int numParticles) :
	CParticleSystem(pGraphicDev)
{
	m_Origin = *origin;
	m_Size = 0.8f;
	m_VBSize = 2048;
	m_VBOffset = 0;
	m_VBBatchSize = 512;

	for (int i = 0; i < numParticles; i++)
		AddParticle();
}

CFirework::CFirework(const CFirework & rhs) :
	CParticleSystem(rhs)
{
	for (auto it = rhs.m_Particles.begin(); it != rhs.m_Particles.end(); it++)
		m_Particles.push_back(*it);
}

CFirework::~CFirework()
{
}

void CFirework::ResetParticle(Particle * particle)
{
	particle->bIsAlive = true;
	particle->vPos = m_Origin;

	_vec3 minVec = _vec3(-1.f, -1.f, -1.f);
	_vec3 maxVec = _vec3(1.f, 1.f, 1.f);

	GetRandomVector(
		&particle->vVelocity,
		&minVec,
		&maxVec);

	D3DXVec3Normalize(
		&particle->vVelocity,
		&particle->vVelocity);

	particle->vVelocity *= 100.f;

	particle->dwColor = D3DXCOLOR(
		GetRandomFloat(0.f, 1.f),
		GetRandomFloat(0.f, 1.f),
		GetRandomFloat(0.f, 1.f),
		1.f);

	particle->fAge = 0.f;
	particle->fLifeTime = 2.0f;
}

_int CFirework::Update_Component(const _float & fTimeDelta)
{
	_int iExit = __super::Update_Component(fTimeDelta);
	if (iExit != 0) return iExit;

	list<Particle>::iterator it;

	for (it = m_Particles.begin(); it != m_Particles.end(); it++)
	{
		if (it->bIsAlive)
		{
			it->vPos += it->vVelocity * fTimeDelta;
			it->fAge += fTimeDelta;

			if (it->fAge > it->fLifeTime)
				it->bIsAlive = false;
		}
		else
		{
			ResetParticle(&*it);
		}
	}
	return 0;
}

CFirework * CFirework::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* origin, int numParticles)
{
	CFirework *	pInstance = new CFirework(pGraphicDev, origin, numParticles);

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CFirework::Clone(void)
{
	return new CFirework(*this);
}

void CFirework::PreRender()
{
	CParticleSystem::PreRender();

	m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, false);
}

void CFirework::PostRender()
{
	CParticleSystem::PostRender();
	m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, true);
}

void CFirework::Free(void)
{
	__super::Free();
}
