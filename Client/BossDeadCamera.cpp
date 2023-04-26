#include "stdafx.h"
#include "BossDeadCamera.h"

CBossDeadCamera::CBossDeadCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CDirectCamera(pGraphicDev), m_bBossDead(false)
{
}

CBossDeadCamera::~CBossDeadCamera()
{
}

HRESULT CBossDeadCamera::Ready_Camera()
{
	m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 60.f,0.1f });
	m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 60.f,1.0f });
	m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 70.f,1.0f });
	m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 0.f,1.0f });
	return S_OK;
}

_int CBossDeadCamera::Update_GameObject(const _float & fTimeDelta)
{
	CGameObject* CBoss = nullptr;

	if (dynamic_cast<CGameObject*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2")))
	{
		CBoss = dynamic_cast<CGameObject*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss2"));
	}
	else if (dynamic_cast<CGameObject*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3")))
	{
		CBoss = dynamic_cast<CGameObject*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3"));
	}

	if (CBoss && !CBoss->Get_Hp())
	{
		m_bBossDead = true;
	}

	if (m_bBossDead)
	{
		if (m_bDead)
			return STAGE_END;

		Update_Direct(fTimeDelta);

		__super::Update_GameObject(fTimeDelta);
		//렌더러에 넣어주기
		Engine::Add_RenderGroup(RENDER_ALPHABLEND, this);
	}

	return 0;
}

CBossDeadCamera * CBossDeadCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBossDeadCamera*		pInstance = new CBossDeadCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Camera()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBossDeadCamera::Free()
{
	__super::Free();
}
