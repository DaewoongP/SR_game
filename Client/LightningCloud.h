#pragma once
#include "Cloud.h"
BEGIN(Engine)
class CRainParticle;
class CRcTex;
class CTexture;
END
class CLightningCloud :
    public CCloud
{
private:
	explicit CLightningCloud(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLightningCloud();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int Update_Too(const _float& fTimeDelta) override;
	virtual _int Update_Top(const _float& fTimeDelta) override;
	virtual void Render_GameObject() override;
	virtual void SwapTrigger();

	virtual void OnCollisionEnter(const class Collision* collsion);
	virtual void OnCollisionStay(const class Collision* collsion);
	virtual void OnCollisionExit(const class Collision* collsion);

	virtual HRESULT		Add_Component(void);
private:
	BoundingBox Check_BoundingBox(size_t ParticleNum);
	void Check_ParticleBox();

	_matrix* MakeLightningMatrix(size_t LightningNum);
	void	CheckPlayerInLightning();
	_bool	CheckRay(_vec3 vPos, _float fLightningSizeY);
public:
	static CLightningCloud* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;

private:
	Engine::CRainParticle*			m_pRainParticle1;
	Engine::CRainParticle*			m_pRainParticle2;
	Engine::CRainParticle*			m_pRainParticle3;
	BoundingBox				m_Box1, m_InitBox1;
	BoundingBox				m_Box2, m_InitBox2;
	BoundingBox				m_Box3, m_InitBox3;
	_bool					m_bInit;

	Engine::CRcTex*					m_pLightningRc1;
	Engine::CRcTex*					m_pLightningRc2;
	Engine::CRcTex*					m_pLightningRc3;
	Engine::CTexture*				m_pLightningTex1;
	Engine::CTexture*				m_pLightningTex2;
	Engine::CTexture*				m_pLightningTex3;

	_vec3					m_vLightningSize1;
	_vec3					m_vLightningSize2;
	_vec3					m_vLightningSize3;
};
