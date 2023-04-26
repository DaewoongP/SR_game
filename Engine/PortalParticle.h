#pragma once
#include "ParticleSystem.h"
BEGIN(Engine)
class CTexture;
class CPortalParticle : public CParticleSystem
{
private:
	explicit CPortalParticle(LPDIRECT3DDEVICE9 pGraphicDev,
		const _tchar* pPath,
		_int iTextureNum = 1,
		_float fSize = 1.f,
		_int iParticleNum = 1,
		_bool isWorld = true,
		_float fLifeTime = 1.f,
		_float fRadius = 10.f);
	explicit CPortalParticle(const CPortalParticle& rhs);
	virtual ~CPortalParticle();
public:
	virtual void ResetParticle(Particle* particle) override;
	virtual _int Update_Particle() override;
	void Set_Options(_float fLife = 1.f, _float fRad = 10.f)
	{
		m_fRadius = fRad;
		m_fLifeTime = fLife;
	}
	void Set_SizeLifeTime(_float sizetime) { m_fSizeoverLifetime = sizetime; }
	void Set_RandomGen(_float fRand)
	{
		m_bSetRandomGenTime = true;
		m_fRand = fRand;
	}
	void Set_Color(_ulong _color) { m_dwColor = _color; }
public:
	static CPortalParticle* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _tchar* pPath,
		_int iTextureNum = 1,
		_float fSize = 1.f,
		_int iParticleNum = 1,
		_bool isWorld = true,
		_float fLifeTime = 1.f,
		_float fRadius = 10.f);
	virtual CComponent* Clone(void) override;

private:
	virtual void Free(void) override;

private:
	_float	m_fLifeTime;
	_float m_fSizeoverLifetime;
	_float	m_fRadius;
	_bool	m_bSetRandomGenTime;
	_float	m_fRand;
	_ulong	m_dwColor;
};

END