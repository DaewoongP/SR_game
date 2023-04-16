#pragma once
#include "Include.h"
#include "GameObject.h"
#include "EarthQuake.h"

class CEarthQuakeSystem :
	public CGameObject
{
private:
	explicit CEarthQuakeSystem(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CEarthQuakeSystem();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void Render_GameObject(void) override;
	virtual void SwapTrigger();

private:
	HRESULT		Add_Component(void);
	void		SummonNewQuake();
	void		CreateQuake(_vec3 vPos,_vec3 dir);
private:
	_float					m_dwLifeTime;
	vector<CEarthQuake*>	m_EarthQuake;
	_int					m_Quakeidx;
	_float					m_QuakeTimer;

public:
	static CEarthQuakeSystem*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;
};

