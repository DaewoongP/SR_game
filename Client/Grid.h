#pragma once

#include "Include.h"
#include "GameObject.h"

BEGIN(Engine)

class CRcCol;

END

class CGrid : public Engine::CGameObject
{
private:
	explicit CGrid(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CGrid();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);

private:
	Engine::CRcCol* m_pBufferCom;

public:
	static CGrid*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void) override;
};

