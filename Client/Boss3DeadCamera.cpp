#include "stdafx.h"
#include "Boss3DeadCamera.h"
#include "StageCamera.h"

CBoss3DeadCamera::CBoss3DeadCamera(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CDirectCamera(pGraphicDev), m_bBossDead(false), m_bSmash(false)
{
}

CBoss3DeadCamera::~CBoss3DeadCamera()
{
}

HRESULT CBoss3DeadCamera::Ready_Camera()
{
	m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 60.f,1.0f });//0
	m_pTransform->m_bUseWeak = true;
	return S_OK;
}

_int CBoss3DeadCamera::Update_GameObject(const _float & fTimeDelta)
{
	CGameObject* CBoss = nullptr;

	if (dynamic_cast<CGameObject*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3")))
	{
		CBoss = dynamic_cast<CGameObject*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3"));
	}

	if (false == m_bBossDead && nullptr != CBoss && 0 == CBoss->Get_Hp())
	{
		m_vecXYPosAngleSpeed.push_back({
			CBoss->m_pTransform->m_vInfo[INFO_POS].x,
			CBoss->m_pTransform->m_vInfo[INFO_POS].y,
			40.f,
			2.f });//갖다대고 1 0.5
		m_vecXYPosAngleSpeed.push_back({
			CBoss->m_pTransform->m_vInfo[INFO_POS].x,
			CBoss->m_pTransform->m_vInfo[INFO_POS].y,
			40.f,
			0.25f });//잠시대기 2 4.0
		m_vecXYPosAngleSpeed.push_back({
			CBoss->m_pTransform->m_vInfo[INFO_POS].x,
			CBoss->m_pTransform->m_vInfo[INFO_POS].y,
			60.f,
			2.f });//준비 3 0.5
		m_vecXYPosAngleSpeed.push_back({
			CBoss->m_pTransform->m_vInfo[INFO_POS].x,
			CBoss->m_pTransform->m_vInfo[INFO_POS].y,
			20.f,
			10.0f });//펀치 4 0.2
		m_vecXYPosAngleSpeed.push_back({
			CBoss->m_pTransform->m_vInfo[INFO_POS].x,
			CBoss->m_pTransform->m_vInfo[INFO_POS].y,
			20.f,
			0.5f });//바사삭 5 2.0
		m_vecXYPosAngleSpeed.push_back({
			CBoss->m_pTransform->m_vInfo[INFO_POS].x,
			CBoss->m_pTransform->m_vInfo[INFO_POS].y,
			60.f,
			0.5f });//돌아와 6
		
		m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 60.f,0.5f });
		m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 60.f,0.5f });
		m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 0.0f,0.5f });
		m_vecXYPosAngleSpeed.push_back({ 31.0f ,17.0f, 0.0f,0.5f });
		m_bBossDead = true;
	}

	if (m_bBossDead)
	{
		if (m_bDead)
			return STAGE_END;

		switch (m_iIndex)
		{
		case 1:
			m_pTransform->Set_Shake(1.0f, 25, SHAKE_Z);
			break;
		case 2:
			m_pTransform->Set_Shake(5.0f, 25, SHAKE_Z);
			break;
		case 3:
			m_pTransform->Set_Shake(0.0f, 0, SHAKE_ALL);
			break;
		case 4:
			
			break;
		case 5:
			if (!m_bSmash)
			{
				m_pTransform->Set_Shake(0.5f, 100, SHAKE_ALL);
				m_bSmash = true;
			}
			break;
		default:
			break;
		}

		Update_Direct(fTimeDelta);

		CCamera::Update_GameObject(fTimeDelta);
	}

	return 0;
}

CBoss3DeadCamera * CBoss3DeadCamera::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBoss3DeadCamera*		pInstance = new CBoss3DeadCamera(pGraphicDev);

	if (FAILED(pInstance->Ready_Camera()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss3DeadCamera::Free()
{
	__super::Free();
}