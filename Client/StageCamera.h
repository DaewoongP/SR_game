#pragma once
#include "Include.h"
#include "GameObject.h"

class CStage1Camera : public Engine::CGameObject
{
public:
	explicit CStage1Camera(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStage1Camera();

public:
	virtual HRESULT Ready_GameObject(void) override;
	virtual _int Update_GameObject(const _float& fTimeDelta) override;
	virtual void LateUpdate_GameObject(void) override;

private:
	void		Rotation_View(const _float& fTimeDelta);
	void		Key_Input(const _float& fTimeDelta);
	void		Move_Camera(const _float& fTimeDelta);


	_float m_fTime;
	_matrix matRotX;
	_vec3 vEye, vAt, vUp;

	_float m_fToo, m_fTop;

public:
	static CStage1Camera*		Create(LPDIRECT3DDEVICE9 pGraphicDev);

	//								�ð�				��					��� ��								�� ���� ��� ����
	void Start_Camera_Shake(_float fDuration, _int iPower, SHAKE_AXIS eShake_Axis = Engine::SHAKE_ALL, _bool bUseWeak =true)
	{
		m_pTransform->m_bUseWeak = bUseWeak;
		m_pTransform->Set_Shake(fDuration, iPower, eShake_Axis);
	}
private:
	virtual void Free();
};
//���������� {������ y + 0 * �Ÿ�, ������ z + 1 * �Ÿ�},{������ y + �ﰢ�Լ�30 * �Ÿ�, ������ z + �ﰢ�Լ�30 * �Ÿ�} * �Ÿ�
//�ٶ󺸴� ���� �׻� ������
