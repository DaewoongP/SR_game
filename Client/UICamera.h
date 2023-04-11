#pragma once
#include "Include.h"
#include "GameObject.h"

class CUICamera : public Engine::CGameObject
{
private:
	explicit CUICamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CUICamera();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;

private:
	
	_matrix m_matProjection;
	_matrix m_matViewSpace;

	
public:
	static CUICamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free();
};