#pragma once
#include "ParticleSystem.h"
BEGIN(Engine)
class CTexture;
class CSuperLandingParticle : public CParticleSystem
{
private:
	explicit CSuperLandingParticle(LPDIRECT3DDEVICE9 pGraphicDev,
		const _tchar* pPath,
		_int iTextureNum = 1,
		_float fSize = 1.f,
		_int iParticleNum = 1,
		_bool isWorld = true);
	explicit CSuperLandingParticle(const CSuperLandingParticle& rhs);
	virtual ~CSuperLandingParticle();
public:
	virtual void ResetParticle(Particle* particle) override;
	virtual _int Update_Particle() override;

public:
	static CSuperLandingParticle* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _tchar* pPath,
		_int iTextureNum = 1,
		_float fSize = 1.f,
		_int iParticleNum = 1,
		_bool isWorld = true);
	virtual CComponent* Clone(void) override;

private:
	virtual void Free(void) override;
private:
	_bool				m_isOdd;
};

END