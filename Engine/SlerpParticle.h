#pragma once
#include "ParticleSystem.h"
BEGIN(Engine)
class CTexture;
class CSlerpParticle : public CParticleSystem
{
private:
	explicit CSlerpParticle(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, _int iTextureNum = 1, _float fSize = 1.f, _int iParticleNum = 1, _bool isWorld = true);
	explicit CSlerpParticle(const CSlerpParticle& rhs);
	virtual ~CSlerpParticle();
public:
	virtual void ResetParticle(Particle* particle) override;
	virtual _int Update_Particle() override;

	void Set_Vectors(_vec3 v1, _vec3 v2)
	{
		m_v1 = v1;
		m_v2 = v2;
	}
	// 0老版快 公力茄
	void Set_LifeTime(_float fLife = 1.f)
	{
		m_fLifeTime = fLife;
		Reset();
	}

public:
	static CSlerpParticle* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, _int iTextureNum = 1, _float fSize = 1.f, _int iParticleNum = 1, _bool isWorld = true);
	virtual CComponent* Clone(void) override;

private:
	virtual void Free(void) override;
private:
	_float		m_fLifeTime;
	_vec3		m_v1;
	_vec3		m_v2;
	_float		m_fTime;
	_vec3		m_vRandUp;
};

END