#pragma once
#include "GameObject.h"
#include "Include.h"

enum GIANTHANDSTATE{
	GH_IDLE,
	GH_STUMP,
	GH_UP,
	GH_END
};

BEGIN(Engine)
class CCollider;
END
class CGiantHand :
	public CGameObject
{
private:
	explicit CGiantHand(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGiantHand();

	virtual HRESULT Ready_GameObject(_vec3& vPos);

	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

private:
	void Do_Stump(const _float & fTimeDelta);
	void Do_Up(const _float & fTimeDelta);

public:
	void	SetState(GIANTHANDSTATE state) { m_eState = state; }

protected:
	Engine::CCollider*		m_pCollider;
	vector<CTransform*>		m_PartsVec;
	_bool					m_bInit;
	_vec3					m_vSummonPos;
	_float					m_fweight;
	GIANTHANDSTATE			m_eState;
protected:
	HRESULT		Add_Component(void);

public:
	static CGiantHand*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);
protected:
	virtual void Free(void) override;

};