#include "stdafx.h"
#include "Snow.h"
#include "Export_Function.h"


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

	particle->vPos.y = m_BoundingBox._max.y;
	
	particle->vVelocity.x = 0.f;
	particle->vVelocity.y = GetRandomFloat(0.f, 1.f) * -5.f;
	particle->vVelocity.z = 0.f;
	
	particle->vTexUV = { 1,1 };
	particle->dwColor = D3DXCOLOR(1.f,1.f,1.f,1.f);
	particle->fAge = 0.f;
	particle->fLifeTime = 10.f;
}

_int CSnow::Update_Component(const _float& fTimeDelta)
{
	_int iExit = __super::Update_Component(fTimeDelta);
	if (iExit != 0) return iExit;

	list<Particle>::iterator it;

	for (it = m_Particles.begin(); it != m_Particles.end(); it++)
	{
		if (it->bIsAlive)
		{
			it->vPos += it->vVelocity * fTimeDelta;

			//영역 이탈시 삭제해주는 친구
			if (m_BoundingBox.Intersect(it->vPos) == false)
			{
				ResetParticle(&(*it));
			}

			it->fAge += fTimeDelta;
			if (it->fAge > it->fLifeTime)
				it->bIsAlive =  false;
		}
		else
			ResetParticle(&(*it));
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
	m_pTexture = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"ParticleTex", m_pGameObject));
	m_pTexture->Add_Anim(L"Idle", 0, 8, 1.f, true);
	m_pTexture->Switch_Anim(L"Idle");
	m_pTexture->m_bUseFrameAnimation = true;
	return new CSnow(*this);
}

void CSnow::Free(void)
{
	__super::Free();
}
