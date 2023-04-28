#pragma once
#include "GameObject.h"
#include "Include.h"
#include "Boss2Parts.h"
BEGIN(Engine)
class CRcTex;
END

class CBoss2Foot :
	public CBoss2Parts
{
public:
	explicit CBoss2Foot(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss2Foot();

	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float& fTimeDelta) override;
	virtual _int Update_Top(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void Render_Too(void) override;
	virtual void Render_Top(void) override;

	void SetAnim(_tchar* name) { m_pTextureCom->Switch_Anim(name); }
private:
	Engine::CRcTex*			m_pBufferCom;

private:
	HRESULT		Add_Component(void);

public:
	static CBoss2Foot*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;
};

