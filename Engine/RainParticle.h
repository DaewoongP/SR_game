#pragma once
#include "ParticleSystem.h"
BEGIN(Engine)
class CTexture;
class CRainParticle : public CParticleSystem
{
private:
	explicit CRainParticle(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, _int iTextureNum = 1, _float fSize = 1.f, _int iParticleNum = 1, _bool isWorld = true);
	explicit CRainParticle(const CRainParticle& rhs);
	virtual ~CRainParticle();
public:
	virtual void ResetParticle(Particle* particle) override;
	virtual _int Update_Particle() override;

	void Set_Offset(_int offset) 
	{ 
		if (1 > offset)
			offset = 1;
		m_iRandOffset = offset;
	}
public:
	static CRainParticle* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath, _int iTextureNum = 1, _float fSize = 1.f, _int iParticleNum = 1, _bool isWorld = true);
	virtual CComponent* Clone(void) override;

private:
	virtual void Free(void) override;
private:
	_int			m_iRandOffset;
};

END