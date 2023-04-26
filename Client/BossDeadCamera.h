#pragma once
#include "DirectCamera.h"
class CBoss2DeadCamera :
	public CDirectCamera
{
private:
	explicit CBoss2DeadCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss2DeadCamera();
	HRESULT Ready_Camera();
	virtual _int Update_GameObject(const _float& fTimeDelta) override;




	_bool m_bBossDead;

public:
	void Set_BossDead(_bool bDead) { m_bBossDead = bDead; }
public:
	static CBoss2DeadCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;
};

