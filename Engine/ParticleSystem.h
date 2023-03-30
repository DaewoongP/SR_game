#pragma once
#include "Component.h"

BEGIN(Engine)

class  CParticleSystem : public CComponent
{
protected:
	explicit CParticleSystem(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CParticleSystem(const CParticleSystem& rhs);
	virtual ~CParticleSystem();
public:
	virtual HRESULT		Ready_Particle(void);
	
	virtual void Reset();
	virtual void ResetParticle(Particle* particle) PURE;
	virtual void AddParticle();
	
	virtual _int Update_Component(const _float& fTimeDelta) override;
	virtual void LateUpdate_Component() override;
	void		Render_Particle(void);

	void SetOrigin(const _vec3& origin) { m_Origin = origin; }
	void SetBoundingBox(const BoundingBox& boundingBox) { m_BoundingBox = boundingBox; }
	void SetEmitRate(const float& emitRate) { m_EmitRate = emitRate; }
	void SetNumParticle(const int& numParticle) 
	{
		Reset();
		for (int i = 0; i < numParticle; i++)
			AddParticle();
	}


	bool IsEmpty();
	bool IsDead();

protected:
	virtual void RemoveDeadParticles();
	virtual void Free(void) override;

	virtual void PreRender();
	virtual void PostRender();

protected:
	_vec3					m_Origin;
	BoundingBox				m_BoundingBox;
	float					m_EmitRate;
	float					m_Size;
	IDirect3DTexture9*		m_Texture;
	LPDIRECT3DVERTEXBUFFER9 m_VB;
	list<Particle>			m_Particles;
	int						m_MaxParticles;

	DWORD					m_VBSize;
	DWORD					m_VBOffset;
	DWORD					m_VBBatchSize;


};

END

