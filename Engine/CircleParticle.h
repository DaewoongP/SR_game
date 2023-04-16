#pragma once
#include "ParticleSystem.h"
BEGIN(Engine)
class CTexture;
class CCircleParticle : public CParticleSystem
{
private:
	explicit CCircleParticle(LPDIRECT3DDEVICE9 pGraphicDev,
		const _tchar* pPath,
		_int iTextureNum = 1,
		_float fSize = 1.f,
		_int iParticleNum = 1,
		_bool isWorld = true,
		_float fLifeTime = 1.f);
	explicit CCircleParticle(const CCircleParticle& rhs);
	virtual ~CCircleParticle();
public:
	virtual void ResetParticle(Particle* particle) override;
	virtual _int Update_Particle() override;
	void Set_Options(_vec3 vUp, _float fLife = 1.f)
	{
		m_vUp = vUp;
		m_fLifeTime = fLife;
	}
public:
	static CCircleParticle* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _tchar* pPath,
		_int iTextureNum = 1,
		_float fSize = 1.f,
		_int iParticleNum = 1,
		_bool isWorld = true,
		_float fLifeTime = 1.f);
	virtual CComponent* Clone(void) override;

private:
	virtual void Free(void) override;

private:
	_float	m_fLifeTime;

	_float	m_fCurRad;
	_vec3	m_vUp;
};

END