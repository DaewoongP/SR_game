#pragma once

#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcTex;
class CTexture;
class CCollider;

END
class CKey : public Engine::CGameObject
{
private:
	explicit CKey(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CKey();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

	virtual void OnTriggerEnter(const class CCollider* other);
	_bool Set_Dead() { return OBJ_DEAD; }
private:
	HRESULT		Add_Component(void);
	//_float		Lerp(_float v0, _float v1, _float t) { return v0 + t * (v1 - v0); }

private:
	Engine::CRcTex* m_pBufferCom;
	Engine::CTexture* m_pTextureCom;
	Engine::CCollider* m_pCollider;

	
	//콜리젼 아님. 어떤 키 눌렀는지 확인용임.
	COL_DIR					m_eKeyState;
public:
	static CKey* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

};

