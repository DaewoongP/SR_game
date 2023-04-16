#pragma once
#include "GameObject.h"
#include "Include.h"
#include "Boss2Parts.h"
BEGIN(Engine)
class CRcTex;
END

class CBoss2Chain :
	public CBoss2Parts
{
public:
	explicit CBoss2Chain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss2Chain();

	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float& fTimeDelta) override;
	virtual _int Update_Top(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;
	virtual void Render_Too(void) override;
	virtual void Render_Top(void) override;

public:
	void	Set_Joint(CTransform* trans01 , CTransform* trans02) { m_pTrans_Joint01 = trans01; m_pTrans_Joint02 = trans02;}
private:
	Engine::CRcTex*			m_pBufferCom;
	
	CTransform* m_pTrans_Joint01;
	CTransform* m_pTrans_Joint02;
private:
	HRESULT		Add_Component(void);
	void		SetChain();

public:
	static CBoss2Chain*		Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos);

private:
	virtual void Free(void) override;
};
