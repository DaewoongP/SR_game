#pragma once
#include "ParticleSystem.h"
BEGIN(Engine)
class CTexture;
class CWaterParticle : public CParticleSystem
{
private:
	explicit CWaterParticle(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, _int iTextureNum = 1, _float fSize = 1.f, _int iParticleNum = 1, _bool isWorld = true);
	explicit CWaterParticle(const CWaterParticle& rhs);
	virtual ~CWaterParticle();
public:
	virtual void ResetParticle(Particle* particle) override;
	virtual _int Update_Particle() override;

	void Set_Offset(_int offset)
	{
		if (1 > offset)
			offset = 1;
		m_iRandOffset = offset;
	}
	void Set_LifeTime(_float fLife = 1.f)
	{
		m_fLifeTime = fLife;
		Reset();
	}
public:
	static CWaterParticle* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, _int iTextureNum = 1, _float fSize = 1.f, _int iParticleNum = 1, _bool isWorld = true);
	virtual CComponent* Clone(void) override;

private:
	virtual void Free(void) override;
private:
	_int			m_iRandOffset;
	_float		m_fLifeTime;
};

END