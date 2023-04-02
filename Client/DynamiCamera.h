#pragma once
#include "Include.h"
#include "GameObject.h"

class CDynamiCamera : public Engine::CGameObject
{
public:
	explicit CDynamiCamera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CDynamiCamera();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;
	virtual void Render_GameObject(void) override;

private:
	HRESULT		Add_Component(void);
	void		Rotation_View();
	void		Key_Input(const _float& fTimeDelta);
	void		Move_Camera(const _float& fTimeDelta);

	float Linear(float A, float B, float Alpha)
	{
		return A * (1 - Alpha) + B * Alpha;
	}

	_float m_fTime;
	_matrix matRotX;
	_vec3 vEye, vAt, vUp;

	_float m_fToo, m_fTop;

public:
	static CDynamiCamera*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

};
//���������� {������ y + 0 * �Ÿ�, ������ z + 1 * �Ÿ�},{������ y + �ﰢ�Լ�30 * �Ÿ�, ������ z + �ﰢ�Լ�30 * �Ÿ�} * �Ÿ�
//�ٶ󺸴� ���� �׻� ������
