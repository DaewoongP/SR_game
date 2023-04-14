#pragma once
#include "ParticleSystem.h"
BEGIN(Engine)
class CTexture;
class CGravityParticle : public CParticleSystem
{
private:
	explicit CGravityParticle(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, _int iTextureNum = 1, _float fSize = 1.f, _int iParticleNum = 1, _bool isWorld = true);
	explicit CGravityParticle(const CGravityParticle& rhs);
	virtual ~CGravityParticle();
public:
	virtual void ResetParticle(Particle* particle) override;
	virtual _int Update_Particle() override;

public:
	static CGravityParticle* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, _int iTextureNum = 1, _float fSize = 1.f, _int iParticleNum = 1, _bool isWorld = true);
	virtual CComponent* Clone(void) override;

private:
	virtual void Free(void) override;
};

END