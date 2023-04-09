#pragma once
#include "Cloud.h"
class CPinkCloud :
	public CCloud
{
private:
	explicit CPinkCloud(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CPinkCloud();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta);
	virtual _int Update_Too(const _float& fTimeDelta) override;
	virtual _int Update_Top(const _float& fTimeDelta) override;
	virtual void Render_Too() override;
	virtual void Render_Top() override;

	virtual HRESULT		Add_Component(void);

public:
	static CPinkCloud*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;
};

