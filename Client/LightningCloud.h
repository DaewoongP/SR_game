
#include "Cloud.h"
#include"Lightning.h"


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
	virtual void Render_Too() override;
	virtual void Render_Top() override;

	virtual HRESULT		Add_Component(void);

public:
	static CLightningCloud* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);
	CLightning* m_pLightning;
private:
	virtual void Free(void) override;


};
