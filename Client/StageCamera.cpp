#include "stdafx.h"
#include "StageCamera.h"
#include "Export_Function.h"

CStage1Camera::CStage1Camera(LPDIRECT3DDEVICE9 pGraphicDev)
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

CStage1Camera::~CStage1Camera()
{
}

HRESULT CStage1Camera::Ready_GameObject(void)
{
	_matrix matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(60), (_float)WINCX / WINCY, 1.0f, 1000.0f);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
	m_pTransform->m_bUseWeak = true;

	return S_OK;
}

_int CStage1Camera::Update_GameObject(const _float & fTimeDelta)
{
	Rotation_View(fTimeDelta);

	Move_Camera(fTimeDelta);

	Key_Input(fTimeDelta);

	__super::Update_GameObject(fTimeDelta);

	return 0;
}

void CStage1Camera::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CStage1Camera::Rotation_View(const _float& fTimeDelta)
{
	_float fRadian;

	fRadian = m_pTransform->m_vAngle.x;

	vEye = { 31.0f ,17.0f,-22.0f };
	vAt = { 31.0f ,17.0f,0.0f };

	D3DXMatrixIdentity(&matRotX);

	vAt.y -= sinf(D3DXToRadian(fRadian))*9.0f;
	vEye.y -= sinf(D3DXToRadian(fRadian))*22.0f;
	vEye.z -= (1 - cosf(D3DXToRadian(fRadian)))*22.0f;

	_vec3 vDir;
	vDir = vEye - vAt;
	D3DXVec3TransformCoord(&vDir, &vDir, &matRotX);

	vEye = vAt + vDir;

	_vec3 vShake = { 0.0f,0.0f,0.0f };
	m_pTransform->Update_Shake(fTimeDelta, vShake);
	
	vEye += vShake;
	vAt += vShake;

	_vec3 vAxisX = { 1.f, 0.f, 0.f };
	// 카메라의 업벡터
	D3DXVec3Cross(&vUp, &(-vDir), &vAxisX);

	_matrix viewMatrix;
	D3DXMatrixLookAtLH(&viewMatrix, &vEye, &vAt, &vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &viewMatrix);
}

void CStage1Camera::Key_Input(const _float & fTimeDelta)
{
	if (Engine::Get_DIKeyState(DIK_X) == Engine::KEYDOWN && (m_fTop <= m_pTransform->m_vAngle.x || m_fToo >= m_pTransform->m_vAngle.x))
	{
		// 2D -> 3D
		if (g_Is2D)
		{
			g_Is2D = false;
		}
		// 3D -> 2D
		else if (!g_Is2D)
		{
			g_Is2D = true;
		}
		
	}

	//카메라 쉐이크 사용 예
	if (Engine::Get_DIKeyState(DIK_F) == Engine::KEYDOWN)
		m_pTransform->Set_Shake(1.0f, 100.0f, SHAKE_Y);
}

void CStage1Camera::Move_Camera(const _float & fTimeDelta)
{
	//투디 일때
	if (g_Is2D)
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
			m_fTime += fTimeDelta * 4.0f;
			
			//각도 감소
			m_pTransform->m_vAngle.x = Lerp(m_fTop, m_fToo, m_fTime);
		}
	}
	else if (!g_Is2D)
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
			m_fTime += fTimeDelta * 4.0f;
			
			//각도 증가
			m_pTransform->m_vAngle.x = Lerp(m_fToo, m_fTop, m_fTime);
		}
	}
}

CStage1Camera * CStage1Camera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage1Camera*		pInstance = new CStage1Camera(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStage1Camera::Free()
{
	__super::Free();
}
