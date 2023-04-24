#pragma once
#include "Tookee.h"
class CThirddee :
	public CTookee
{
private:
	explicit CThirddee(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CThirddee();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float& fTimeDelta) override;
	virtual _int Update_Top(const _float& fTimedDelte) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void SwapTrigger();

	virtual void OnCollisionEnter(const class Collision* collision);
	virtual void OnCollisionStay(const class Collision* collision);
	virtual void OnCollisionExit(const class Collision* collision);

	void	Set_Die();
private:
	virtual void		Key_Input(const _float& fTimeDelta) override;
	void				DoFlip() override;
	void				Spiwn_End(const _float& fTimeDelta);
	
	//삼디는 키 입력이 다름.
	virtual void		Key_Input2(const _float& fTimeDelta);
	HRESULT		Add_Component(void);

private:
	_float	m_fTopOffset;
	Engine::CAnimation*     m_pAnimation_Head;
	_bool					m_EndingTrigger;

public:
	static CThirddee*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

protected:
	virtual void Free(void) override;
};

