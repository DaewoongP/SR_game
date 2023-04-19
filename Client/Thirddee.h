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
	_int Update_Too(const _float& fTimeDelta) override;
	virtual _int Update_Top(const _float& fTimedDelte) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void Render_Too(void) override;
	virtual void Render_Top(void) override;
	virtual void SwapTrigger();

	virtual void OnCollisionEnter(const class Collision* collision);
	virtual void OnCollisionStay(const class Collision* collision);
	virtual void OnCollisionExit(const class Collision* collision);

private:
	virtual void		Key_Input(const _float& fTimeDelta) override;
	HRESULT		Add_Component(void);

private:
	Engine::CAnimation*     m_pAnimation_Head;

public:
	static CThirddee*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

protected:
	virtual void Free(void) override;
};

