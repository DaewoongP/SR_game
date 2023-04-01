#include "stdafx.h"
#include "DynamiCamera.h"
#include "Export_Function.h"



CDynamiCamera::CDynamiCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev),
	m_bToodee(true),
	m_fTime(0.0f),
	vEye({ 32.0f ,18.0f,-30.0f}),
	vAt({ 32.0f ,18.0f,0.0f }),
	vUp({ 0.0f ,1.0f,0.0f })
{
}

CDynamiCamera::~CDynamiCamera()
{
}

HRESULT CDynamiCamera::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->Set_Pos(0.0f, 0.0f, -100.0f);

	m_pTransform->m_vInfo[INFO_LOOK] = { 0.0f,0.0f,1.0f };
	_matrix ProjectionMatrix;
	D3DXMatrixPerspectiveFovLH(&ProjectionMatrix, D3DXToRadian(60), 960.0f / 600.0f, 0.0f, 1000.0f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &ProjectionMatrix);
	return S_OK;
}

_int CDynamiCamera::Update_GameObject(const _float & fTimeDelta)
{
	_float fTemp;

	fTemp = m_pTransform->m_vAngle.x;
	fTemp *= -1;
	vEye = { 32.0f ,18.0f,-25.0f };
	vAt = { 32.0f ,18.0f,0.0f };
	
	D3DXMatrixIdentity(&matRotX);

	D3DXMatrixRotationX(&matRotX, D3DXToRadian(fTemp));

	_vec3 vDir;
	vDir = vEye - vAt;
	D3DXVec3TransformCoord(&vDir, &vDir, &matRotX);

	vEye = vAt + vDir;
	_vec3 vAxisX = { 1.f, 0.f, 0.f };
	// ī�޶��� ������
	D3DXVec3Cross(&vUp, &(-vDir), &vAxisX);
	_matrix viewMatrix;

	D3DXMatrixLookAtLH(&viewMatrix, &vEye, &vAt, &vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &viewMatrix);

	ToodeeAndTopdee(fTimeDelta);

	Key_Input(fTimeDelta);

	__super::Update_GameObject(fTimeDelta);	

	return 0;
}

void CDynamiCamera::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CDynamiCamera::Render_GameObject(void)
{
}

HRESULT CDynamiCamera::Add_Component(void)
{
	CComponent*		pComponent = nullptr;
	
	return S_OK;
}

void CDynamiCamera::Key_Input(const _float & fTimeDelta)
{
	if (30 <= m_pTransform->m_vAngle.x || 1 >= m_pTransform->m_vAngle.x)
	{
		if (GetAsyncKeyState('T'))
		{
			if (m_bToodee)
			{
				m_bToodee = false;
			}
			else if (!m_bToodee)
			{
				m_bToodee = true;
			}
		}
	}
}

void CDynamiCamera::ToodeeAndTopdee(const _float & fTimeDelta)
{
	//���� �϶�
	if (m_bToodee)
	{
		//������ 1���� �۴ٸ�
		if (1.0f >= m_pTransform->m_vAngle.x)
		{
			//x�� ȸ�� 1��
			m_pTransform->m_vAngle.x = 1.0f;
			//�ð� �ʱ�ȭ
			m_fTime = 0.0f;
			m_bToodee = false;
		}
		//1���� ũ�ٸ�
		else
		{
			//�ð� ���� 
			m_fTime += fTimeDelta;
			
			//���� ����
			m_pTransform->m_vAngle.x = Linear(30.0f, 1.0f, m_fTime);

			m_pTransform->Rotation(ROT_X, D3DXToRadian(m_pTransform->m_vAngle.x));

			_vec3 vPos = { 0.0f, 0.0f, -100.0f };

			_matrix matRotX, matTrans;

			D3DXMatrixRotationX(&matRotX, D3DXToRadian(m_pTransform->m_vAngle.x));

			D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);

			vPos = { 0.0f, 0.0f, 0.0f };

			matRotX *= matTrans;

			D3DXVec3TransformCoord(&vPos, &vPos, &matRotX);

			m_pTransform->Set_Pos(vPos.x, vPos.y, vPos.z);
		}
	}
	else if (!m_bToodee)
	{
		//������ 30���� ũ�ٸ�
		if (30.0f <= m_pTransform->m_vAngle.x)
		{
			//x�� ȸ�� 30��
			m_pTransform->m_vAngle.x = 30.0f;
			//�ð� �ʱ�ȭ
			m_fTime = 0.0f;
			m_bToodee = true;

		}
		//30���� �۴ٸ�
		else
		{
			//�ð� ���� 
			m_fTime += fTimeDelta;
			
			//���� ����
			m_pTransform->m_vAngle.x = Linear(1.0f, 30.0, m_fTime);

			//�θ� ��Ȱ�� �� matrix
			_matrix matParents;

			D3DXMatrixRotationX(&matParents, m_pTransform->m_vAngle.x);
		}
	}
}

CDynamiCamera * CDynamiCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDynamiCamera*		pInstance = new CDynamiCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}
