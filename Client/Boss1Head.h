#pragma once
#include "GameObject.h"
#include "Include.h"

BEGIN(Engine)
class CCollider;
class CAnimation;
END
class CBoss1Head :
	public CGameObject
{
private:
	explicit CBoss1Head(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss1Head();

	virtual HRESULT Ready_GameObject(_vec3& vPos, _vec3 vToWard);

	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

	//이동과 동시에 도달했는지를 반환해주는 친구.
	_bool	isMoveDone(const _float & fTimeDelta);
	void	ShootHead(const _float & fTimeDelta);

	void		LerpClipAdd(AnimClip* clip, _int idx, _float itv, _float osc, _float csc, _vec3 otr, _vec3 ctr, _vec3 orot, _vec3 crot, _int count);
protected:
	Engine::CCollider*		m_pCollider;
	vector<CTransform*>		m_PartsVec;
	Engine::CAnimation*     m_pAnimation_Whole;
	_bool					m_bInit;

	//자식도 있는 그냥 애니메이션 넣어줌.
	_float m_fMovepos[3];
	_int   m_iShootCount;

	_float m_fSpeed;

protected:
	HRESULT		Add_Component(void);

public:
	static CBoss1Head*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _vec3 & vtoward);
protected:
	virtual void Free(void) override;

};