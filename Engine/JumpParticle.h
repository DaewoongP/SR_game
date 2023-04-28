#pragma once
#include "ParticleSystem.h"
BEGIN(Engine)
class CTexture;
class CJumpParticle : public CParticleSystem
{
private:
	explicit CJumpParticle(LPDIRECT3DDEVICE9 pGraphicDev, 
		const _tchar* pPath,
		_int iTextureNum = 1, 
		_float fSize = 1.f,
		_int iParticleNum = 1,
		_bool isWorld = true);
	explicit CJumpParticle(const CJumpParticle& rhs);
	virtual ~CJumpParticle();
public:
	virtual void ResetParticle(Particle* particle) override;
	virtual _int Update_Particle() override;
	void Set_LiftTime(_float lifetime) { m_fLifeTime = lifetime; }
	void Set_SizeLifeTime(_float size) { m_fSizeoverLifetime = size; }
	void Set_Color(_ulong dwColor) { m_dwColor = dwColor; }
public:
	static CJumpParticle* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _tchar* pPath,
		_int iTextureNum = 1,
		_float fSize = 1.f, 
		_int iParticleNum = 1,
		_bool isWorld = true);
	virtual CComponent* Clone(void) override;

private:
	virtual void Free(void) override;
private:
	_float		m_fSizeoverLifetime;
	_float		m_fLifeTime;
	_ulong		m_dwColor;
};

END