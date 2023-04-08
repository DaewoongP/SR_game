#pragma once
#include"Cube.h"
BEGIN(Engine)
class CCubeTex;
class CTexture;
class CCollider;
END	

class CCrackBlock :
    public CCube
{
private:
	explicit CCrackBlock(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCrackBlock();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual _int Update_Too(const _float& fTimeDelta) override;
	virtual _int Update_Top(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void LateUpdate_Too() override;
	virtual void LateUpdate_Top() override;
	virtual void Render_GameObject(void) override;
	virtual void Render_Too() override;
	virtual void Render_Top() override;
	virtual void OnCollisionEnter(const class Collision* collision);
	/*virtual void OnCollisionStay(const class Collision* collision);
	virtual void OnCollisionExit(const class Collision* collision);*/

	
	
private:
	HRESULT Add_Component(void);
	_bool m_bRealDead;
private:
	Engine::CCubeTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CCollider* m_pCollider;

private:
	//블럭이 사라지는 시간 *.*초
	_float m_fBlockTime =2.5f;
	void Shaking(_vec3& vPos,const _float& fTimeDelta);
	
public:
	static CCrackBlock* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};

