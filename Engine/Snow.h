#pragma once
#include "ParticleSystem.h"

BEGIN(Engine)

class  CSnow : public CParticleSystem
{
public:
	explicit CSnow(LPDIRECT3DDEVICE9 pGraphicDev, BoundingBox* boundingBox, int numParticles);
	explicit CSnow(const CSnow& rhs);
	virtual ~CSnow();

	virtual void ResetParticle(Particle* particle) override;


	virtual _int Update_Component(const _float& fTimeDelta) override;


	static CSnow* Create(LPDIRECT3DDEVICE9 pGraphicDev, BoundingBox* boundingBox, int numParticles);
	virtual CComponent* Clone(void) override;

protected:
	virtual void Free(void) override;

};

END

