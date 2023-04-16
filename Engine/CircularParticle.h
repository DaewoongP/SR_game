#pragma once
#include "ParticleSystem.h"
BEGIN(Engine)
class CTexture;
class CCircularParticle : public CParticleSystem
{
private:
	explicit CCircularParticle(LPDIRECT3DDEVICE9 pGraphicDev,
		const _tchar* pPath,
		_int iTextureNum = 1,
		_float fSize = 1.f,
		_int iParticleNum = 1,
		_bool isWorld = true,
		_float fLifeTime = 1.f,
		_float fRadius = 10.f);
	explicit CCircularParticle(const CCircularParticle& rhs);
	virtual ~CCircularParticle();
public:
	virtual void ResetParticle(Particle* particle) override;
	virtual _int Update_Particle() override;
	void Set_Options(_float fLife = 1.f, _float fRad = 10.f)
	{
		m_fRadius = fRad;
		m_fLifeTime = fLife;
	}
	void Set_SizeLifeTime(_float sizetime) { m_fSizeoverLifetime = sizetime; }
	void Set_RandomGen() { m_bSetRandomGenTime = true; }
	void Set_Color(_ulong _color) { m_dwColor = _color; }
public:
	static CCircularParticle* Create(LPDIRECT3DDEVICE9 pGraphicDev, 
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
	_ulong	m_dwColor;
};

END