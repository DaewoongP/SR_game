#pragma once
#include "DirectCamera.h"
class CBossDeadCamera :
	public CDirectCamera
{
private:
	explicit CBossDeadCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBossDeadCamera();
	HRESULT Ready_Camera();
	virtual _int Update_GameObject(const _float& fTimeDelta) override;




	_bool m_bBossDead;

public:
	void Set_BossDead(_bool bDead) { m_bBossDead = bDead; }
public:
	static CBossDeadCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;
};

