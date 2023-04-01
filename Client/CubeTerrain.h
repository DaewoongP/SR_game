#pragma once

#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CCubeTex;
class CTexture;

END
class CCubeTerrain : public Engine::CGameObject
{
private:
	explicit CCubeTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCubeTerrain();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);
	void		Key_Input();

private:
	Engine::CCubeTex*			m_pBufferCom;
	Engine::CTexture*		m_pTextureCom;

public:
	void	Set_CubeIndex(int iCubeIndex) { m_iCubeIndex = iCubeIndex; }

private:
	int m_iCubeIndex;

public:
	static CCubeTerrain*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;

};

