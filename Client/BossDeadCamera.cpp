#include "stdafx.h"
#include "BossDeadCamera.h"

CBoss2DeadCamera::CBoss2DeadCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CDirectCamera(pGraphicDev), m_bBossDead(false)
{
}

CBoss2DeadCamera::~CBoss2DeadCamera()
{
}

HRESULT CBoss2DeadCamera::Ready_Camera()
{
	m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 60.f,1.0f });
	
	return S_OK;
}

_int CBoss2DeadCamera::Update_GameObject(const _float & fTimeDelta)
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

	if (false == m_bBossDead && nullptr != CBoss && 0 == CBoss->Get_Hp())
	{
		m_vecXYPosAngleSpeed.push_back({
			CBoss->m_pTransform->m_vInfo[INFO_POS].x - 2.0f,
			CBoss->m_pTransform->m_vInfo[INFO_POS].y,
			30.f,
			10.0f });
		m_vecXYPosAngleSpeed.push_back({
			CBoss->m_pTransform->m_vInfo[INFO_POS].x - 2.0f,
			CBoss->m_pTransform->m_vInfo[INFO_POS].y,
			30.f,
			1.1f });
		m_vecXYPosAngleSpeed.push_back({ 
			CBoss->m_pTransform->m_vInfo[INFO_POS].x - 2.0f + 15.0f,
			CBoss->m_pTransform->m_vInfo[INFO_POS].y + 30.0f,
			40.f,
			2.0f });
		m_vecXYPosAngleSpeed.push_back({
			CBoss->m_pTransform->m_vInfo[INFO_POS].x - 2.0f + 15.0f,
			CBoss->m_pTransform->m_vInfo[INFO_POS].y + 30.0f,
			40.f,
			1.0f });
		m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 60.f,1.0f });
		m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 60.f,1.0f });
		m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 0.0f,1.0f });
		m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 0.0f,1.0f });

		m_bBossDead = true;
	}

	if (m_bBossDead)
	{
		if (m_bDead)
			return STAGE_END;

		Update_Direct(fTimeDelta);

		CCamera::Update_GameObject(fTimeDelta);
	}

	return 0;
}

CBoss2DeadCamera * CBoss2DeadCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoss2DeadCamera*		pInstance = new CBoss2DeadCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Camera()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss2DeadCamera::Free()
{
	__super::Free();
}
