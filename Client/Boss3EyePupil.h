#pragma once
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;

END

class CBoss3EyePupil : public CGameObject
{
private:
	explicit CBoss3EyePupil(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss3EyePupil();
	virtual HRESULT Ready_GameObject(_vec3& vPos, _int iIndex);
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT Add_Component(void);
	void	LookAtPlayer();

private:
	Engine::CRcTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;

	CGameObject*			m_pBoss3;

	_int					m_iIndex;	// 표정 텍스처 구분

public:
	static CBoss3EyePupil*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _int iIndex);

protected:
	virtual void Free(void) override;
};

