#pragma once
#include "Camera.h"
class CBoss2Camera :
	public CCamera
{
private:
	explicit CBoss2Camera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CBoss2Camera();
public:
	HRESULT Ready_Camera();
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
private:
	// x, y ������ ����  
	// angle Ȯ��
	// �ӵ�
	vector<_vec4> m_vecXYPosAngleSpeed;
	_int m_iIndex;
	_float m_fTimer;

private:
	void Update_Direct(const _float& fTimeDelta);
	
public:
	static CBoss2Camera*		Create(LPDIRECT3DDEVICE9 pGraphicDev);
private:
	virtual void Free() override;
};

