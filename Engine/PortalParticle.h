#pragma once
#include "ParticleSystem.h"
class CPortalParticle : public CParticleSystem
{
public:
	CPortalParticle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPortalParticle();
};

