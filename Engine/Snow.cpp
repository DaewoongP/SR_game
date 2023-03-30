#include "stdafx.h"
#include "Snow.h"



CSnow::CSnow(LPDIRECT3DDEVICE9 pGraphicDev, BoundingBox* boundingBox, int numParticles) :
	CParticleSystem(pGraphicDev)
{
	m_BoundingBox = *boundingBox;
	m_Size = 0.8f;
	m_VBSize = 2048;
	m_VBOffset = 0;
	m_VBBatchSize = 512;

	for (int i = 0; i < numParticles; i++)
		AddParticle();
}

CSnow::CSnow(const CSnow& rhs) :
	CParticleSystem(rhs)
{
	for (auto it = rhs.m_Particles.begin(); it != rhs.m_Particles.end(); it++)
		m_Particles.push_back(*it);
}

CSnow::~CSnow()
{

}

void CSnow::ResetParticle(Particle* particle)
{
	particle->bIsAlive = true;

	GetRandomVector(
		&particle->vPos, 
		&m_BoundingBox._min, 
		&m_BoundingBox._max);

	/*particle->vPos.y = m_BoundingBox._max.y;*/

	
	particle->vVelocity.x = 0.f;
	particle->vVelocity.y = GetRandomFloat(0.f, 1.0f) * -10.f;
	particle->vVelocity.z = 0.f;

	particle->dwColor = D3DXCOLOR(1.f,1.f,1.f,0.8f);
}

_int CSnow::Update_Component(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Component(fTimeDelta);
	if (iExit != 0) return iExit;

	list<Particle>::iterator it;

	for (it = m_Particles.begin(); it != m_Particles.end(); it++)
	{
		it->vPos += it->vVelocity * fTimeDelta;

		if (m_BoundingBox.Intersect(it->vPos) == false)
		{
			ResetParticle(&(*it));
		}
	}
	return 0;
}

CSnow* CSnow::Create(LPDIRECT3DDEVICE9 pGraphicDev, BoundingBox* boundingBox, int numParticles)
{
	CSnow *	pInstance = new CSnow(pGraphicDev, boundingBox, numParticles);

	if (FAILED(pInstance->Ready_Particle()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

Engine::CComponent* CSnow::Clone(void)
{
	return new CSnow(*this);
}

void CSnow::Free(void)
{
	__super::Free();
}
