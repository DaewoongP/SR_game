#pragma once
#include "GameObject.h"
#include "Include.h"

class CBoss2Parts :
	public CGameObject
{
public:
	explicit CBoss2Parts(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss2Parts();

	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float& fTimeDelta) override;
	virtual _int Update_Top(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void Render_Too(void) override;
	virtual void Render_Top(void) override;

public:
	void Blink_White();

private:
	_float	m_dwTimer;

private:
	HRESULT		Add_Component(void);

public:
	static CBoss2Parts*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

protected:
	virtual void Free(void) override;
};

