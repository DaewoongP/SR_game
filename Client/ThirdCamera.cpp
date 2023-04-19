#include "stdafx.h"
#include "ThirdCamera.h"


CThirdCamera::CThirdCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}


CThirdCamera::~CThirdCamera()
{
}

HRESULT CThirdCamera::Ready_GameObject(void)
{
	D3DXMatrixIdentity(&m_matRot);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
	m_fs = 0.f;
	m_fCamChangeSpeed = 1.5f;
	return S_OK;
}

_int CThirdCamera::Update_GameObject(const _float & fTimeDelta)
{
	Set_PlayerPos();
	Update_Matrix();
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProj);

	Swap_Camera(fTimeDelta);
	return 0;
}

void CThirdCamera::Swap_Camera(const _float& fTimeDelta)
{
	if (Engine::Get_DIKeyState(DIK_X) == Engine::KEYDOWN)
	{
		m_fs = 0.f;
		g_Is2D = !g_Is2D;
	}

	if (g_Is2D)
		Set_ToodeeCam(fTimeDelta);
	else
		Set_TopdeeCam(fTimeDelta);

	m_fs += fTimeDelta * m_fCamChangeSpeed;
	if (m_fs > 1.f)
		m_fs = 1.f;
}

void CThirdCamera::Set_ToodeeCam(const _float& fTimeDelta)
{
	_vec3 vUp;
	for (size_t i = 0; i < 3; ++i)
	{
		D3DXVec3Cross(&vUp, &m_vTopView[i], &m_vTooView[i]);
		GetVectorSlerp(&m_vViewInfo[i], &m_vTopView[i], &m_vTooView[i], &vUp, 20.f, m_fs);
	}
}

void CThirdCamera::Set_TopdeeCam(const _float& fTimeDelta)
{
	_vec3 vUp;
	for (size_t i = 0; i < 3; ++i)
	{
		D3DXVec3Cross(&vUp, &m_vTooView[i], &m_vTopView[i]);
		GetVectorSlerp(&m_vViewInfo[i], &m_vTooView[i], &m_vTopView[i], &vUp, 20.f, m_fs);
	}
}

void CThirdCamera::Update_Matrix()
{
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(45.f), (_float)WINCX / WINCY, 1.f, 100.f);
	D3DXMatrixLookAtLH(&m_matView, &m_vViewInfo[EYE], &m_vViewInfo[AT], &m_vViewInfo[UP]);
}

void CThirdCamera::Set_PlayerPos()
{
	CGameObject* pObj = nullptr;
	pObj = Engine::Get_GameObject(L"Layer_GameLogic", L"Thirddee");
	if (pObj == nullptr)
		return;
	m_vPlayerPos = pObj->m_pTransform->m_vInfo[INFO_POS];

	m_vTooView[EYE] = m_vPlayerPos + _vec3(-15.f, 0.f, -10.f);
	m_vTooView[AT] = m_vPlayerPos;
	m_vTooView[UP] = _vec3(0.f, 1.f, 0.f);

	m_vTopView[EYE] = m_vPlayerPos + _vec3(-15.f, 0.f, -8.f);
	m_vTopView[AT] = m_vPlayerPos;
	m_vTopView[UP] = _vec3(0.f, 0.f, -1.f);
}

CThirdCamera * CThirdCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CThirdCamera*		pInstance = new CThirdCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CThirdCamera::Free(void)
{
	__super::Free();
}
