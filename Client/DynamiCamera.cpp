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
	_float fTemp;

	fTemp = m_pTransform->m_vAngle.x;
	fTemp *= -1;
	vEye = { 32.0f ,18.0f,-25.0f };
	vAt = { 32.0f ,18.0f,0.0f };
	
	D3DXMatrixIdentity(&matRotX);

	D3DXMatrixRotationX(&matRotX, D3DXToRadian(fTemp));

	vEye.y -= sinf(D3DXToRadian(fTemp))*25.0f;
	vEye.z -= (1 - cosf(D3DXToRadian(fTemp)))*25.0f;

	_vec3 vDir;
	vDir = vEye - vAt;
	D3DXVec3TransformCoord(&vDir, &vDir, &matRotX);

	vEye = vAt + vDir;



	_vec3 vAxisX = { 1.f, 0.f, 0.f };
	// 카메라의 업벡터
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
	//투디 일때
	if (Is2D)
	{
		//각도가 1보다 작다면
		if (m_fToo >= m_pTransform->m_vAngle.x)
		{
			//x축 회전 1도
			m_pTransform->m_vAngle.x = m_fToo;
			//시간 초기화
			m_fTime = 0.0f;
			//Is2D = false;
		}
		//1보다 크다면
		else
		{
			//시간 누적 
			m_fTime += fTimeDelta;
			
			//각도 감소
			m_pTransform->m_vAngle.x = Linear(m_fTop, m_fToo, m_fTime);

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
	else if (!Is2D)
	{
		//각도가 30보다 크다면
		if (m_fTop <= m_pTransform->m_vAngle.x)
		{
			//x축 회전 30도
			m_pTransform->m_vAngle.x = m_fTop;
			//시간 초기화
			m_fTime = 0.0f;
			//Is2D = true;

		}
		//30보다 작다면
		else
		{
			//시간 누적 
			m_fTime += fTimeDelta;
			
			//각도 증가
			m_pTransform->m_vAngle.x = Linear(m_fToo, m_fTop, m_fTime);

			//부모 역활을 할 matrix
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
