#pragma once
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;

END
class CShotingPlayer :
	public CGameObject
{
private:
	explicit CShotingPlayer(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CShotingPlayer();

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
	void		Key_Input(const _float& fTimeDelta);
	HRESULT		Add_Component(void);

private:
	_float	m_fTopOffset;
	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;

public:
	static CShotingPlayer*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

protected:
	virtual void Free(void) override;
};
