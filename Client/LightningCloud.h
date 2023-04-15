#pragma once
#include "Cloud.h"
BEGIN(Engine)
class CRainParticle;

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

	virtual HRESULT		Add_Component(void);

	BoundingBox Check_BoundingBox();
	void Check_ParticleBox();
public:
	static CLightningCloud* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;

private:
	CRainParticle*			m_pRainParticle;
	BoundingBox				m_Box, m_InitBox;
	_bool					m_bInit;
};
