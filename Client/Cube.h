#pragma once
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CCubeTex;
class CTexture;
class CCollider;

END

class CCube :
	public CGameObject
{
protected:
	explicit CCube(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCube();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void OnCollisionEnter(const class Collision* collision);
	virtual void OnCollisionStay(const class Collision* collision);
	virtual void OnCollisionExit(const class Collision* collision);

	void Set_CubeTexNum(_int iNum) { m_iCubeTexNum = iNum; }
	void Set_EarthQuake(_float fDuration, _int iPower, SHAKE_AXIS eShake_Axis = Engine::SHAKE_ALL, _bool bUseWeak = true)
	{
		m_pTransform->m_bUseWeak = bUseWeak;
		m_pTransform->Set_Shake(fDuration, iPower, eShake_Axis);
	}
protected:
	HRESULT Add_Component(void);

protected:
	Engine::CCubeTex*	m_pBufferCom;
	Engine::CTexture*	m_pTextureCom;
	Engine::CTexture*	m_pTextureCom2;
	Engine::CCollider * m_pCollider;

	_int				m_iCubeTexNum;
public:
	static CCube*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _int iIndex = 0);

protected:
	virtual void Free(void) override;
};

