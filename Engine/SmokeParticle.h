#pragma once
#include "ParticleSystem.h"
BEGIN(Engine)
class CTexture;
class CSmokeParticle : public CParticleSystem
{
private:
	explicit CSmokeParticle(LPDIRECT3DDEVICE9 pGraphicDev,
		const _tchar* pPath,
		_int iTextureNum = 1,
		_float fSize = 1.f,
		_int iParticleNum = 1,
		_bool isWorld = true);
	explicit CSmokeParticle(const CSmokeParticle& rhs);
	virtual ~CSmokeParticle();
public:
	virtual void ResetParticle(Particle* particle) override;
	virtual _int Update_Particle() override;

private:
	_float				m_fGenTime;
	_float				m_fParticleNum;

public:
	static CSmokeParticle* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _tchar* pPath,
		_int iTextureNum = 1,
		_float fSize = 1.f,
		_int iParticleNum = 1,
		_bool isWorld = true);
	virtual CComponent* Clone(void) override;

private:
	virtual void Free(void) override;
};

END