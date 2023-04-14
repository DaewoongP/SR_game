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

	virtual HRESULT Ready_GameObject(_vec3& vPos, _int iIndex);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float & fTimeDelta);
	virtual _int Update_Top(const _float & fTimeDelta);
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT Add_Component(void);

private:
	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;

	CGameObject*			m_pParentBoss3;

	_int					m_iIndex;	// 표정 텍스처 구분
	_float					m_fXAngle;	// 플레이어 전환 시 x축 회전 값

public:
	static CBoss3Eye*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _int iIndex);

protected:
	virtual void Free(void) override;
};

