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
#include "Toodee.h"

CFinalStage3::CFinalStage3(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev), m_bMonkeySpwanTrigger(true)
{
}


CFinalStage3::~CFinalStage3()
{
}

HRESULT CFinalStage3::Ready_Scene(void)
{
	m_SpwanCube = false;
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
	if (m_pBoss->m_iHp <= 99.f && !m_SpwanCube && m_bMonkeySpwanTrigger)
	{
		m_StageState = F3_SpawnCube;
		m_bMonkeySpwanTrigger = false;
	}
		
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

HRESULT CFinalStage3::Ready_Layer_Environment(const _tchar* pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	FAILED_CHECK_RETURN(FACTORY<CStage1Camera>::Create(L"Camera", pLayer), E_FAIL);
	//FAILED_CHECK_RETURN(FACTORY<CDynamicCamera>::Create(L"ShootingCamera", pLayer), E_FAIL);
	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CFinalStage3::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;
	FAILED_CHECK_RETURN(FACTORY<CStarBox>::Create(L"StarBox", pLayer), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CShootingPlayer>::Create(L"Thirddee", pLayer, _vec3(0.f, 0.f, 15.f)), E_FAIL);

	pGameObject = m_pBoss = CFinal3Boss1::Create(m_pGraphicDev, _vec3(0.f, 200.f, 30.f));
	pLayer->Add_GameObject(L"Final3Boss1", pGameObject);
	///////////////////////// 1¹ø ////////////////////////////////////
	/*
	FAILED_CHECK_RETURN(FACTORY<CToodee>::Create(L"Toodee", pLayer, _vec3(58.f, 6.f, 10.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CTopdee>::Create(L"Topdee", pLayer, _vec3(26.f, 10.f, 10.f)), E_FAIL);

	FAILED_CHECK_RETURN(FACTORY<CFinalStoneCube>::CreateParent(L"StoneCube", pLayer, _vec3(0,0,0)), E_FAIL);*/
	///////////////////////// 2¹ø ////////////////////////////////////
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
	}
}

void CFinalStage3::ActionNone(const _float & fTimeDelta)
{
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