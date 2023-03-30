#pragma once
#include "ParticleSystem.h"

BEGIN(Engine)
class  CFirework : public CParticleSystem
{
public:
	explicit CFirework(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* origin, int numParticles);
	explicit CFirework(const CFirework& rhs);
	virtual ~CFirework();

	virtual void ResetParticle(Particle* particle) override;


	virtual _int Update_Component(const _float& fTimeDelta) override;


	static CFirework* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3* origin, int numParticles);
	virtual CComponent* Clone(void) override;

private:
	virtual void PreRender() override;
	virtual void PostRender() override;

protected:
	virtual void Free(void) override;
};
END

