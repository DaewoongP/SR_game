#pragma once
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;

END

class CBoss3Eye : public CGameObject
{
private:
	explicit CBoss3Eye(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss3Eye();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos, _int iIndex);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

	void	Set_Damaged() { m_bDamaged = true; }

private:
	HRESULT Add_Component(void);
	void	DamagedBoss3(const _float& fTimeDelta);

private:
	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;

	CGameObject*			m_pBoss3;
	_int					m_iIndex;	// 표정 텍스처 구분

	_bool					m_bDamaged;
	_float	m_fDamagedTime;

public:
	static CBoss3Eye*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _int iIndex);

protected:
	virtual void Free(void) override;
};

