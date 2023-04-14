#pragma once
#include "ParticleSystem.h"
BEGIN(Engine)
class CTexture;
class CWindParticle : public CParticleSystem
{
private:
	explicit CWindParticle(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, _int iTextureNum = 1, _float fSize = 1.f, _int iParticleNum = 1, _bool isWorld = true);
	explicit CWindParticle(const CWindParticle& rhs);
	virtual ~CWindParticle();
public:
	virtual void ResetParticle(Particle* particle) override;
	virtual _int Update_Particle() override;
	// 0老版快 公力茄
	void Set_LifeTime(_float fLife = 1.f)
	{
		m_fLifeTime = fLife;
		Reset();
	}

public:
	static CWindParticle* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, _int iTextureNum = 1, _float fSize = 1.f, _int iParticleNum = 1, _bool isWorld = true);
	virtual CComponent* Clone(void) override;

private:
	virtual void Free(void) override;
private:
	_float		m_fLifeTime;
};

END