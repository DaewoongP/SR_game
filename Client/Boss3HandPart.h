#pragma once
#include"..\Engine\Include.h"
#include"..\Engine\GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
END
class CBoss3HandPart :
    public CGameObject
{
private:
	explicit CBoss3HandPart(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss3HandPart();

public:
	void	Set_SparkOn(_bool tf) { m_bSparkON = tf; }
	void	Set_Damaged() { m_bDamaged = true; }
public:
	virtual HRESULT Ready_GameObject(_vec3& vPos,_int iIndex);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float& fTimeDelta) override;
	virtual _int Update_Top(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	//void Rot2Hand();

private:
	HRESULT		Add_Component(void);
	void	DamagedBoss3(const _float& fTimeDelta);

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CTexture* m_pTextureCom2;

	Engine::CTexture* m_pCreateTextureCom;
	Engine::CTexture* m_pSparkAnimation;
	Engine::CCollider* m_pCollider;

	CGameObject* m_pBoss3;
	CGameObject* m_pBoss3LPart;
	CGameObject* m_pBoss3RPart;

	_int	m_iIndex;

	_bool  m_bSetTarget;
	_float m_fSpeed;
	_vec3  m_vDir;
	_bool  m_bSparkON;

	_bool					m_bDamaged;
	_float	m_fDamagedTime;
	_int	m_bWhiteIndex;

public:
	static CBoss3HandPart* Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos,_int iIndex);

private:
	virtual void Free(void) override;

};

