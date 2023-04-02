#include "stdafx.h"
#include "DynamiCamera.h"
#include "Export_Function.h"



CDynamiCamera::CDynamiCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev),
	m_fTime(0.0f),
	vEye({ 32.0f ,18.0f,-30.0f}),
	vAt({ 32.0f ,18.0f,0.0f }),
	vUp({ 0.0f ,1.0f,0.0f }),
	m_fToo(0.0f),
	m_fTop(30.0f)
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
	D3DXMatrixPerspectiveFovLH(&ProjectionMatrix, D3DXToRadian(60), (float)WINCX / WINCY, 1.0f, 1000.0f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &ProjectionMatrix);
	return S_OK;
}

_int CDynamiCamera::Update_GameObject(const _float & fTimeDelta)
{
	_float fRadian;

	fRadian = m_pTransform->m_vAngle.x;
	
	vEye = { 31.0f ,17.0f,-22.0f };
	vAt = { 31.0f ,17.0f,0.0f };
	
	D3DXMatrixIdentity(&matRotX);

	//D3DXMatrixRotationX(&matRotX, D3DXToRadian(fRadian));

	_float fTemp;
	fTemp = sinf(D3DXToRadian(fRadian));

	vAt.y -= sinf(D3DXToRadian(fRadian))*9.0f;
	

	vEye.y -= sinf(D3DXToRadian(fRadian))*22.0f;
	
	vEye.z -= (1 - cosf(D3DXToRadian(fRadian)))*22.0f;

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
	
		if (GetAsyncKeyState('T') & 0x8000 && (m_fTop <= m_pTransform->m_vAngle.x || m_fToo >= m_pTransform->m_vAngle.x))
		{
			if (Is2D)
			{
				Is2D = false;
			}
			else if (!Is2D)
			{
				Is2D = true;
			}
		}
}

void CDynamiCamera::ToodeeAndTopdee(const _float & fTimeDelta)
{
	//���� �϶�
	if (Is2D)
	{
		//������ 1���� �۴ٸ�
		if (m_fToo >= m_pTransform->m_vAngle.x)
		{
			//x�� ȸ�� 1��
			m_pTransform->m_vAngle.x = m_fToo;
			//�ð� �ʱ�ȭ
			m_fTime = 0.0f;
			//Is2D = false;
		}
		//1���� ũ�ٸ�
		else
		{
			//�ð� ���� 
			m_fTime += fTimeDelta * 4.0f;
			
			//���� ����
			m_pTransform->m_vAngle.x = Linear(m_fTop, m_fToo, m_fTime);
		}
	}
	else if (!Is2D)
	{
		//������ 30���� ũ�ٸ�
		if (m_fTop <= m_pTransform->m_vAngle.x)
		{
			//x�� ȸ�� 30��
			m_pTransform->m_vAngle.x = m_fTop;
			//�ð� �ʱ�ȭ
			m_fTime = 0.0f;
			//Is2D = true;

		}
		//30���� �۴ٸ�
		else
		{
			//�ð� ���� 
			m_fTime += fTimeDelta * 4.0f;
			
			//���� ����
			m_pTransform->m_vAngle.x = Linear(m_fToo, m_fTop, m_fTime);
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
