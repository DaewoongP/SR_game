#pragma once
#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CCubeTex;
class CTexture;

END

class CTexCube :
	public CGameObject
{
private:
	explicit CTexCube(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTexCube();

public:
	virtual HRESULT Ready_GameObject(_vec3& vPos) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

	void Set_CubeTexNum(_int iNum) { m_iCubeTexNum = iNum; }
private:
	HRESULT Add_Component(void);

private:
	Engine::CCubeTex*	m_pBufferCom;
	Engine::CTexture*	m_pTextureCom;
	Engine::CTexture*	m_pTextureCom2;

	_int				m_iCubeTexNum;
public:
	static CTexCube*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _int iIndex = 0);

private:
	virtual void Free(void) override;
};

