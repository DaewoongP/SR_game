#include "stdafx.h"
#include "FinalStage3.h"
#include "AbstractFactory.h"
#include "Export_Function.h"
#include "Cube.h"
#include "ImguiMgr.h"
#include "ImguiStage.h"
#include "ImguiUnit.h"
#include "Fade.h"
#include "DynamicCamera.h"
#include "Final3Boss1.h"
#include "StarBox.h"
#include "ShootingCamera.h"
#include "ShootingPlayer.h"
#include"FinalUI.h"
#include"FinalUI_B.h"
#include "FinalMonkeyCube.h"
#include "Item.h"	
#include "StageCamera.h"
#include "Boss2.h"
#include "Item.h"
#include "FinalStoneCube.h"
#include "Boss3.h"
#include "Topdee.h"
#include "Thirddee.h"
#include "Toodee.h"

CFinalStage3::CFinalStage3(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev), m_bMonkeySpawnTrigger(true), m_bStoneSpawnTrigger(false)
{
}


CFinalStage3::~CFinalStage3()
{
}

HRESULT CFinalStage3::Ready_Scene(void)
{
	m_SpwanCube = false;
	m_ShootingPlayerLerpTrigger = false;
	m_SwapTop_ShootingTirgger = false;
	m_eLoadingID = LOADING_FINAL3;
	m_pBoss = nullptr;
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	funcAction.push_back(&CFinalStage3::SpawnCube_Monkey);
	funcAction.push_back(&CFinalStage3::Check_Cube_Place);
	funcAction.push_back(&CFinalStage3::MonkeyDisAppear);
	funcAction.push_back(&CFinalStage3::ActionNone);
	m_StageState = F3_NONE;
	return S_OK;
}

_int CFinalStage3::Update_Scene(const _float & fTimeDelta)
{
	if (m_pBoss->m_iHp <= 99.f && !m_SpwanCube && m_bMonkeySpawnTrigger)
	{
		CLayer* pLayer = Engine::Get_Layer(L"Layer_Environment");
		pLayer->Delete_Tag(L"ShootingCamera");
		FACTORY<CStage1Camera>::Create(L"Camera", pLayer);
		m_StageState = F3_SpawnCube;
		m_bMonkeySpawnTrigger = false;
		m_ShootingPlayerLerpTrigger = true;
		dynamic_cast<CShootingPlayer*>(m_ShootingPlayer)->Set_Shoot(false);
	}
	Do_SwapPlayer(fTimeDelta);
	DoLerpShootingPlayer(fTimeDelta);
	(this->*funcAction[m_StageState])(fTimeDelta);
	return __super::Update_Scene(fTimeDelta);
}

void CFinalStage3::LateUpdate_Scene(void)
{
	__super::LateUpdate_Scene();
}

void CFinalStage3::Render_Scene(void)
{
}

void CFinalStage3::Do_SwapPlayer(const _float & fTimeDelta)
{
	if (m_SwapTop_ShootingTirgger)
	{
		//탑디가 중앙으로 올라감.
		m_TooTop->m_pTransform->m_vInfo[INFO_POS] = Lerp(m_TooTop->m_pTransform->m_vInfo[INFO_POS], _vec3(CUBEX, CUBEY, 10), fTimeDelta);
		if (D3DXVec3Length(&(m_TooTop->m_pTransform->m_vInfo[INFO_POS] - _vec3(CUBEX, CUBEY, 10))) < 0.3f)
		{
			m_TooTop->Set_Render(false);
			m_TooTop->Set_Update(false);
			m_ShootingPlayer->Set_Render(true);
			m_ShootingPlayer->Set_Update(true);
			m_ShootingPlayer->m_pTransform->m_vInfo[INFO_POS] = _vec3(CUBEX, CUBEY, 10);
		}
		
	}
}

HRESULT CFinalStage3::Ready_Layer_Environment(const _tchar* pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	
	FAILED_CHECK_RETURN(FACTORY<CShootingCamera>::Create(L"ShootingCamera", pLayer), E_FAIL);
	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CFinalStage3::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;
	FAILED_CHECK_RETURN(FACTORY<CStarBox>::Create(L"StarBox", pLayer), E_FAIL);

	m_ShootingPlayer = CShootingPlayer::Create(m_pGraphicDev, _vec3(0.f, 0.f, 15.f));
	pLayer->Add_GameObject(L"Topdee", m_ShootingPlayer);

	pGameObject = m_pBoss = CFinal3Boss1::Create(m_pGraphicDev, _vec3(0.f, 200.f, 30.f));
	pLayer->Add_GameObject(L"Final3Boss1", pGameObject);
	for (int i = 0; i < CUBEX; i++)
	{
		if (i % 10 < 4)
			continue;
		FAILED_CHECK_RETURN(FACTORY<CFinalMonkeyCube>::Create(L"MapCube", _vec3{ (_float)i * 2,10.f,10.f }, m_MokeyCube, 2), E_FAIL);
	}
	for (int i = 0; i < CUBEY; i++)
	{
		for (int j = 0; j < CUBEX; j++)
		{
			if (i == 0)
				FAILED_CHECK_RETURN(FACTORY<CFinalMonkeyCube>::Create(L"MapCube", _vec3{ (_float)j * 2,(_float)i * 2,10.f }, m_MokeyCube, 2), E_FAIL);
			if (i == CUBEY - 1)
				FAILED_CHECK_RETURN(FACTORY<CFinalMonkeyCube>::Create(L"MapCube", _vec3{ (_float)j * 2,(_float)i * 2,10.f }, m_MokeyCube, 2), E_FAIL);
			if (j == 0)
				FAILED_CHECK_RETURN(FACTORY<CFinalMonkeyCube>::Create(L"MapCube", _vec3{ (_float)j * 2,(_float)i * 2,10.f }, m_MokeyCube, 2), E_FAIL);
			if (j == CUBEX - 1)
				FAILED_CHECK_RETURN(FACTORY<CFinalMonkeyCube>::Create(L"MapCube", _vec3{ (_float)j * 2,(_float)i * 2,10.f }, m_MokeyCube, 2), E_FAIL);
		}
	}
	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CFinalStage3::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	_int pHp = Get_GameObject(L"Layer_GameLogic", L"Final3Boss1")->Get_Hp();
	FAILED_CHECK_RETURN(FACTORY<CFinalUI_B>::Create(L"FinalUI", pLayer, pHp), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CFinalUI>::Create(L"FinalUI", pLayer, pHp), E_FAIL);

	m_uMapLayer.insert({ pLayerTag, pLayer });
	return S_OK;
}

void CFinalStage3::SpawnCube_Monkey(const _float& fTimeDelta)
{
	auto pStageLayer = m_uMapLayer.find(L"Layer_Environment");
	if (pStageLayer == m_uMapLayer.end())
		return;

	for (int i = 0; i < m_MokeyCube.size(); i++)
		pStageLayer->second->Add_GameObject(m_MokeyCube[i]->m_pTag, m_MokeyCube[i]);
	m_StageState = F3_MonkeyAppear;
	m_SpwanCube = true;
}

void CFinalStage3::Check_Cube_Place(const _float& fTimeDelta)
{
	if (m_SpwanCube)
	{
		int _cnt = 0;
		CFinalMonkeyCube* cube = nullptr;
		for (int i = 0; i< m_MokeyCube.size(); i++)
		{
			cube = dynamic_cast<CFinalMonkeyCube*>(m_MokeyCube[i]);
			if (cube)
				if (cube->Get_MoveDone())_cnt = _cnt; else _cnt++;
		}
		if (_cnt == 0)
			MonkeyAppear(fTimeDelta);
	}
}

void CFinalStage3::MonkeyAppear(const _float& fTimeDelta)
{
	if (m_pMonkey != nullptr)
		return;
	auto pStageLayer = m_uMapLayer.find(L"Layer_GameLogic");
	if (pStageLayer != m_uMapLayer.end())
		FACTORY<CBoss2>::Create(L"Boss2", pStageLayer->second, _vec3(54.f, 18.f, 10.f), 1);
	m_pMonkey = pStageLayer->second->Get_GameObject(L"Boss2");
	m_StageState = F3_Destroy;
}

void CFinalStage3::MonkeyDisAppear(const _float& fTimeDelta)
{
	if (m_pMonkey == nullptr)
		return;
	if (m_pMonkey->m_pTransform->m_vInfo[INFO_POS].x < 0)
	{
		for (int i = 0; i < m_MokeyCube.size(); i++)
			dynamic_cast<CFinalMonkeyCube*>(m_MokeyCube[i])->SetLerpPos(200, GetRandomFloat(0, 2.f),true);
		m_pMonkey = nullptr;
		m_StageState = F3_NONE;

		CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
		_vec3 pos = m_TooTop->m_pTransform->m_vInfo[INFO_POS];
		m_TooTop->Set_Render(false);
		m_TooTop->Set_Update(false); 
		m_TooTop = nullptr;
		m_TooTop = CTopdee::Create(m_pGraphicDev, pos);
		pLayer->Add_GameObject(L"Topdee", m_TooTop);

		FACTORY<CFinalStoneCube>::CreateParent(L"StoneCube", pLayer, _vec3(0, 0, 0));
	}
}

void CFinalStage3::ActionNone(const _float & fTimeDelta)
{
}

void CFinalStage3::DoLerpShootingPlayer(const _float & fTimeDelta)
{
	if (m_ShootingPlayerLerpTrigger)
	{
		m_ShootingPlayer->m_pTransform->m_vInfo[INFO_POS] = Lerp(m_ShootingPlayer->m_pTransform->m_vInfo[INFO_POS], _vec3(10.f, 14.f, 10.f), fTimeDelta*1);
		m_ShootingPlayer->m_pTransform->m_vAngle = Lerp(m_ShootingPlayer->m_pTransform->m_vAngle, _vec3(0, 0, 0), fTimeDelta);
		if (D3DXVec3Length(&(m_ShootingPlayer->m_pTransform->m_vInfo[INFO_POS] - _vec3(10.f, 14.f, 10.f)))<0.1f)
		{
			CLayer* pLayer = Engine::Get_Layer(L"Layer_GameLogic");
			m_TooTop = CToodee::Create(m_pGraphicDev, _vec3(10.f, 14.f, 10.f));
			pLayer->Add_GameObject(L"Topdee", m_TooTop);
			m_ShootingPlayer->Set_Render(false);
			m_ShootingPlayer->Set_Update(false);
			m_ShootingPlayerLerpTrigger = false;
		}
	}
}

CFinalStage3 * CFinalStage3::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CFinalStage3 *	pInstance = new CFinalStage3(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFinalStage3::Free(void)
{
	__super::Free();
}