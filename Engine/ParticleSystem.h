#pragma once
#include "Component.h"

BEGIN(Engine)
class CTexture;
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
	// 업데이트 파티클 안에 렌더를 돌릴수있게 해놨음.
	// 사실 이거를 분리하는게 맞는데 귀찮아서 걍 때려박음.
	// 나중에 분리하려면 각 자식객체 파티클 클래스에서 렌더를 따로불러주면됨.
	virtual _int	Update_Particle();
	void			Render_Particle(void);

	void Set_Origin(const _vec3& origin) { m_vOrigin = origin; }
	void Set_BoundingBox(const BoundingBox& boundingBox)
	{
		m_BoundingBox = boundingBox;
		Reset();
	}
	void SetEmitRate(const float& emitRate) { m_EmitRate = emitRate; }
	void SetNumParticle(const int& numParticle)
	{
		Reset();
		for (int i = 0; i < numParticle; i++)
			AddParticle();
	}

	bool IsEmpty();
	bool IsDead();

	void Start_Particle() 
	{ 
		Reset();
		m_bTrigger = true;
	}
	void End_Particle() 
	{
		m_bTrigger = false; 
	}
	void Set_Size(_float fSize) { m_Size = fSize; }
	void Set_AnimSpeed(_float fSpeed) { m_fAnimSpeed = fSpeed; }

	void Set_World(_matrix mat) { m_matWorld = mat; }
protected:
	virtual void RemoveDeadParticles();
	virtual void Free(void) override;

	virtual void PreRender();
	virtual void PostRender();

protected:
	_vec3					m_vOrigin;
	BoundingBox				m_BoundingBox;
	float					m_EmitRate;
	float					m_Size;
	LPDIRECT3DVERTEXBUFFER9 m_VB;
	list<Particle>			m_Particles;
	int						m_MaxParticles;
	DWORD					m_dwFVF;
	DWORD					m_VBSize;
	DWORD					m_VBOffset;
	DWORD					m_VBBatchSize;

	CTexture*				m_pTexture;

	_bool					m_bIsWorld;
	_bool					m_bTrigger;

	_float					m_fAnimSpeed;
	_bool					m_bChangedTex;
	_matrix					m_matWorld;

	Particle*				m_pParticle;
};

END

