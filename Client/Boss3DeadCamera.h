#pragma once
#include "DirectCamera.h"
class CBoss3DeadCamera :
	public CDirectCamera
{
private:
	explicit CBoss3DeadCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss3DeadCamera();
	HRESULT Ready_Camera();
	virtual _int Update_GameObject(const _float& fTimeDelta) override;

	_bool m_bBossDead;

	_bool m_bSmash;

public:
	void Set_BossDead(_bool bDead) { m_bBossDead = bDead; }
public:
	static CBoss3DeadCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;
	
};