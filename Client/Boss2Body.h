#pragma once
#include "GameObject.h"
#include "Include.h"

BEGIN(Engine)
class CRcTex;
class CTexture;
END

class CBoss2Body :
	public CGameObject
{
public:
	explicit CBoss2Body(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss2Body();

	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float& fTimeDelta) override;
	virtual _int Update_Top(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void Render_Too(void) override;
	virtual void Render_Top(void) override;

public:
	void SetMoveAble(_bool moveable) { m_bIsMoveable = moveable; }
	void SetRotAngle(_float min, _float max) { 
		m_fMin = min; 
		m_fMax = max; }
private:
	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;

private:
	_float		m_fMin;
	_float		m_fMax;
	_float		m_BounusAngle_z;
	_bool		m_bIsMoveable;
	_bool		m_bDir;
	HRESULT		Add_Component(void);

public:
	static CBoss2Body*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;
};

