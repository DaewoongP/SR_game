#include "stdafx.h"
#include "Camera.h"

#include "Component.h"

#include "Export_Function.h"


CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev), m_bSwitch(false),
	m_pName(nullptr)
{
	//ZeroMemory(m_szName, sizeof(_tchar) * 256);
	ZeroMemory(&m_tViewParams, sizeof(VIEWPARAMS));
	ZeroMemory(&m_tProjParams, sizeof(PROJPARAMS));
}

CCamera::CCamera(const CCamera & rhs)
	:CComponent(rhs), m_bSwitch(false),
	m_pName(rhs.m_pName)
{
	//ZeroMemory(m_szName, sizeof(_tchar) * 256);
	m_tViewParams = rhs.m_tViewParams;
	m_tProjParams = rhs.m_tProjParams;
	m_matView = rhs.m_matView;
	m_matProj = rhs.m_matProj;
}

CCamera::~CCamera()
{
}

HRESULT CCamera::Ready_Camera(VIEWPARAMS& tViewParam, PROJPARAMS& tProjParam)
{
	m_tViewParams = tViewParam;
	m_tProjParams = tProjParam;

	m_tViewParams.LookAtLH(&m_matView);
	m_tProjParams.PerspectiveLH(&m_matProj);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	return S_OK;
}

_int CCamera::Update_Component(const _float & fTimeDelta)
{
	if (!m_bSwitch)
		return 0;
	
	m_tViewParams.vEye = m_pGameObject->m_pTransform->m_vInfo[INFO_POS];

	m_tViewParams.vAt = m_tViewParams.vEye + m_pGameObject->m_pTransform->m_vInfo[INFO_LOOK];

	m_tViewParams.LookAtLH(&m_matView);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);

	return 0;
}

CCamera * CCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, VIEWPARAMS& tViewParam, PROJPARAMS& tProjParam)
{
	CCamera* pInstance = new CCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Camera(tViewParam, tProjParam)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CCamera::Clone(void)
{
	CCamera* pInstance = new CCamera(*this);
	NULL_CHECK_RETURN(pInstance, nullptr);

	// 여기서 카메라 매니저에 추가.
	//Engine::CCameraMgr::GetInstance()->Add_Camera(m_pName, pInstance);

	return pInstance;
}

void CCamera::Free()
{
	// 여기서 카메라 매니저에 제거.
	//Engine::CCameraMgr::GetInstance()->Remove_Camera(m_pName);

	__super::Free();
}

#if 0
#include "stdafx.h"
#include "Camera.h"

#include "Component.h"

#include "Export_Function.h"


CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev), m_hWnd(nullptr), m_pvTarget(nullptr), m_iTest(2)
{
	ZeroMemory(&m_tViewParams, sizeof(VIEWPARAMS));
	ZeroMemory(&m_tProjParams, sizeof(PROJPARAMS));
}

CCamera::CCamera(const CCamera & rhs)
	:CComponent(rhs),
	m_hWnd(rhs.m_hWnd), m_pvTarget(rhs.m_pvTarget), m_iTest(2)
{
	m_tViewParams = rhs.m_tViewParams;
	m_tProjParams = rhs.m_tProjParams;
	m_matView = rhs.m_matView;
	m_matProj = rhs.m_matProj;
}

CCamera::~CCamera()
{
}

HRESULT CCamera::Ready_Camera(VIEWPARAMS& tViewParam, PROJPARAMS& tProjParam, HWND& hWnd)
{
	m_tViewParams = tViewParam;
	m_tProjParams = tProjParam;
	m_hWnd = hWnd;

	m_tViewParams.LookAtLH(&m_matView);
	m_tProjParams.PerspectiveLH(&m_matProj);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);







	return S_OK;
}

_int CCamera::Update_Component(const _float & fTimeDelta)
{
	Key_Input(fTimeDelta);

	if (m_bFix)
	{
		Fix_Mouse();
		Mouse_Move();
	}
	First_Person_View();



	return 0;
}

void CCamera::Key_Input(const _float & fTimeDelta)
{
	_matrix		matCamWorld;
	D3DXMatrixInverse(&matCamWorld, 0, &m_matView);

	if (Engine::Get_DIKeyState(DIK_W) & 0x80)
	{
		_vec3 vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3 vLength = *D3DXVec3Normalize(&vLook, &vLook) * fTimeDelta * 10.f;

		m_tViewParams.vEye += _vec3(0.f, 0.f, vLength.z);
	}

	if (Engine::Get_DIKeyState(DIK_S) & 0x80)
	{
		_vec3 vLook;
		memcpy(&vLook, &matCamWorld.m[2][0], sizeof(_vec3));

		_vec3 vLength = *D3DXVec3Normalize(&vLook, &vLook) * fTimeDelta * 10.f;

		m_tViewParams.vEye -= _vec3(0.f, 0.f, vLength.z);
	}

	if (Engine::Get_DIKeyState(DIK_A) & 0x80)
	{
		_vec3		vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * fTimeDelta * 10.f;

		m_tViewParams.vEye -= _vec3(vLength.x, 0.f, 0.f);
	}

	if (Engine::Get_DIKeyState(DIK_D) & 0x80)
	{
		_vec3		vRight;
		memcpy(&vRight, &matCamWorld.m[0][0], sizeof(_vec3));

		_vec3	vLength = *D3DXVec3Normalize(&vRight, &vRight) * fTimeDelta * 10.f;

		m_tViewParams.vEye += _vec3(vLength.x, 0.f, 0.f);
	}

	/*if (Engine::Get_DIKeyState(DIK_SPACE) & 0x80)
	{
		_vec3	vLength = _vec3(0.f, 1.f, 0.f) * fTimeDelta * 10.f;

		m_tViewParams.vEye += vLength;
		m_tViewParams.vAt += vLength;
	}*/
}

void CCamera::Mouse_Move()
{
	_long dwMouseMove = 0;

	_matrix matCamWorld;
	D3DXMatrixInverse(&matCamWorld, 0, &m_matView);

	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_Y))
	{
		_vec3 vRight;
		memcpy(&vRight, &matCamWorld.m[0], sizeof(_vec3));

		_matrix matRot;
		D3DXMatrixRotationAxis(&matRot, &vRight, D3DXToRadian(dwMouseMove) / 10.f);

		_vec3 vLook = m_tViewParams.vAt - m_tViewParams.vEye;
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_tViewParams.vAt = m_tViewParams.vEye + vLook;
	}

	if (dwMouseMove = Engine::Get_DIMouseMove(DIMS_X))
	{
		_matrix matRot;
		_vec3 vUp = { 0.f, 1.f, 0.f };

		D3DXMatrixRotationAxis(&matRot, &vUp, D3DXToRadian(dwMouseMove / 10.f));

		_vec3 vLook = m_tViewParams.vAt - m_tViewParams.vEye;
		D3DXVec3TransformNormal(&vLook, &vLook, &matRot);

		m_tViewParams.vAt = m_tViewParams.vEye + vLook;
	}
}

void CCamera::Fix_Mouse()
{
	POINT ptMouse{ WINCX >> 1, WINCY >> 1 };

	ClientToScreen(m_hWnd, &ptMouse);
	SetCursorPos(ptMouse.x, ptMouse.y);
}

void CCamera::First_Person_View()
{
	CTransform* pTransform = m_pGameObject->m_pTransform;

	_vec3 vDir = pTransform->m_vInfo[INFO_LOOK];

	D3DXVec3Normalize(&vDir, &vDir);

	m_tViewParams.vEye = pTransform->m_vInfo[INFO_POS];

	m_tViewParams.vAt = pTransform->m_vInfo[INFO_POS] + vDir;

	m_tViewParams.LookAtLH(&m_matView);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
}

void CCamera::Third_Person_View()
{
	CTransform* pTransform = m_pGameObject->m_pTransform;

	m_tViewParams.vAt = pTransform->m_vInfo[INFO_POS];

	_vec3 vDir = pTransform->m_vInfo[INFO_LOOK];

	D3DXVec3Normalize(&vDir, &vDir);

	m_tViewParams.vEye = (pTransform->m_vInfo[INFO_POS] - vDir * 10.f);

	m_tViewParams.LookAtLH(&m_matView);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
}

void CCamera::Ready_Target(_vec3 * Target)
{
	m_pvTarget = Target;
}

void CCamera::Target_View(_vec3 * Target)
{
	if (nullptr == Target)
	{
		return;
	}
	m_tViewParams.vAt = *Target;

	m_tViewParams.vEye = m_pGameObject->m_pTransform->m_vInfo[INFO_POS];

	m_tViewParams.LookAtLH(&m_matView);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
}

void CCamera::Test_Change_View()
{
	if (GetAsyncKeyState('F'))
	{
		++m_iTest;
		if (3 <= m_iTest)
		{
			m_iTest = 0;
		}
	}

	switch (m_iTest)
	{
	case 0:
		First_Person_View();
		break;
	case 1:
		Third_Person_View();
		break;
	case 2:
	{
		_vec3 Test{ 0.0f, 0.0f, 0.0f };
		Ready_Target(&Test);
		Target_View(&Test);
	}
	break;

	default:
		break;
	}




}

CCamera * CCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev, VIEWPARAMS& tViewParam, PROJPARAMS& tProjParam, HWND hWnd)
{
	CCamera* pInstance = new CCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Camera(tViewParam, tProjParam, hWnd)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

CComponent * CCamera::Clone(void)
{
	return new CCamera(*this);
}

void CCamera::Free()
{
	__super::Free();
}

#endif // 0
