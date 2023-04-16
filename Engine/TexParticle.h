#pragma once
#include "ParticleSystem.h"
BEGIN(Engine)
class CTexture;
class CTexParticle : public CParticleSystem
{
private:
	explicit CTexParticle(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, _int iTextureNum = 1, _float fSize = 1.f, _int iParticleNum = 1, _bool isWorld = true);
	explicit CTexParticle(const CTexParticle& rhs);
	virtual ~CTexParticle();
public:
	virtual void ResetParticle(Particle* particle) override;
	virtual _int Update_Particle() override;

	void Set_SizeLifeTime(_float size) { m_fSizeoverLifetime = size; }
public:
	static CTexParticle* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, _int iTextureNum = 1, _float fSize = 1.f, _int iParticleNum = 1, _bool isWorld = true);
	virtual CComponent* Clone(void) override;

private:
	virtual void Free(void) override;
private:
	_float		m_fSizeoverLifetime;
};

END