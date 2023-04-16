#pragma once
#include "GameObject.h"
#include "Include.h"
#include "Boss2Parts.h"
BEGIN(Engine)
class CRcTex;
class CRigidbody;
class CShadow;
END

#define BOSETAILPARTWAVESTART -20
#define BOSETAILPARTWAVEEND 20

class CBoss2Tail :
	public CBoss2Parts
{
public:
	explicit CBoss2Tail(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss2Tail();

	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float& fTimeDelta) override;
	virtual _int Update_Top(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void Render_Too(void) override;
	virtual void Render_Top(void) override;

	void Set_PreTail(CTransform* PreTail) { m_pPreTail = PreTail; }

private:
	Engine::CRcTex*			m_pBufferCom;
	Engine::CRigidbody*		m_pRigid;
	Engine::CShadow*		m_pShadow;


	CTransform* m_pPreTail;

	_bool m_bMetronome = true;

	_float m_fStartZ;

	_float m_fZRatio;
private:
	HRESULT		Add_Component(void);

public:
	static CBoss2Tail*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);
	_float m_fTimer = 0.0f;

private:
	virtual void Free(void) override;
};

