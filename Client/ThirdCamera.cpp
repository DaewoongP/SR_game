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
	m_fShakeValue = 0;
	D3DXMatrixIdentity(&m_matRot);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
	m_fs = 0.f;
	m_fCamChangeSpeed = 1.5f;
	return S_OK;
}

_int CThirdCamera::Update_GameObject(const _float & fTimeDelta)
{
	Set_PlayerPos(fTimeDelta);
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
		if (g_Is2D)
		{
			Engine::StopSound(SOUND_CAM);
			Engine::PlaySound_Effect(L"71.wav", SOUND_CAM, 0.5f);
		}
		else
		{
			Engine::StopSound(SOUND_CAM);
			Engine::PlaySound_Effect(L"70.wav", SOUND_CAM, 0.5f);
		}
		m_fs = 0.f;
		g_Is2D = !g_Is2D;

	}

	if (Engine::Get_DIKeyState(DIK_F5) == Engine::KEYDOWN)
	{
		g_IsInvin = !g_IsInvin;
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
	D3DXMatrixPerspectiveFovLH(&m_matProj, D3DXToRadian(45.f), (_float)WINCX / WINCY, 1.f, 500.f);
	D3DXMatrixLookAtLH(&m_matView, &m_vViewInfo[EYE], &m_vViewInfo[AT], &m_vViewInfo[UP]);
}

void CThirdCamera::Set_PlayerPos(const _float& fTimeDelta)
{
	CGameObject* pObj = nullptr;
	pObj = Engine::Get_GameObject(L"Layer_GameLogic", L"Thirddee");
	if (pObj == nullptr)
		return;
	m_vPlayerPos = pObj->m_pTransform->m_vInfo[INFO_POS] + _vec3(0,0,-10);

	_vec3 vShake = { 0.0f,0.0f,0.0f };

	m_pTransform->Update_Shake(fTimeDelta, vShake);


	m_vTooView[EYE] = m_vPlayerPos + _vec3(-15.f, 0.f, -10.f);
	m_fShakeValue = Lerp(m_fShakeValue, 0, 0.1f);
	m_vTooView[AT] = m_vPlayerPos+ _vec3(GetRandomFloat(-m_fShakeValue, m_fShakeValue), GetRandomFloat(-m_fShakeValue, m_fShakeValue), GetRandomFloat(-m_fShakeValue, m_fShakeValue));
	m_vTooView[UP] = _vec3(0.f, 1.f, 0.f);

	m_vTopView[EYE] = m_vPlayerPos + _vec3(-15.f, 0.f, -8.f);
	m_vTopView[AT] = m_vPlayerPos + vShake;
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
