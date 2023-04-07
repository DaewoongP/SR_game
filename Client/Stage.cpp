#include "stdafx.h"
#include "Stage.h"

#include "Export_Function.h"
#include "Monster.h"
#include "SkyBox.h"
#include "Cube.h"
#include "Grid.h"
#include "DynamiCamera.h"
#include "MoveBox.h"
#include "Player02.h"
#include "Pig.h"
#include "Bat.h"
#include"Key.h"
#include"KeyBox.h"
#include "StageBG.h"


CStage::CStage(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}


CStage::~CStage()
{
}

HRESULT CStage::Ready_Scene(void)
{
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

_int CStage::Update_Scene(const _float & fTimeDelta)
{
	return __super::Update_Scene(fTimeDelta);
}

void CStage::LateUpdate_Scene(void)
{
	__super::LateUpdate_Scene();
}

void CStage::Render_Scene(void)
{

}


HRESULT CStage::Ready_Layer_Environment(const _tchar* pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;
	//CAMERA
	pGameObject = CDynamiCamera::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Camera", pGameObject), E_FAIL);

	pGameObject = CStageBG::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"StageBG", pGameObject), E_FAIL);

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{

	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	//Key
	pGameObject = CKey::Create(m_pGraphicDev, _vec3(10.f, 10.f, 10.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Key", pGameObject), E_FAIL);

	//KeyBox
	pGameObject = CKeyBox::Create(m_pGraphicDev, _vec3(20.f, 20.f, 10.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"KeyBox", pGameObject), E_FAIL);
  
	//Pig
	pGameObject = CPig::Create(m_pGraphicDev, _vec3(50.f, 7.f, 10.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pig_0", pGameObject), E_FAIL);
  
	// PLAYER
	pGameObject = CPlayer::Create(m_pGraphicDev, _vec3(10.f, 7.f, 10.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);

	// PLAYER
	pGameObject = CPlayer02::Create(m_pGraphicDev, _vec3(16.f, 10.f, 11.f));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player02", pGameObject), E_FAIL);

	pGameObject = CMoveBox::Create(m_pGraphicDev, _vec3(20, 10, 10));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoveCube", pGameObject), E_FAIL);

	pGameObject = CMoveBox::Create(m_pGraphicDev, _vec3(30, 20, 10));
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoveCube", pGameObject), E_FAIL);

	int cubeCnt = 0;
	for (int i = 0; i < CUBEY; i++)
	{
		for (int j = 0; j < CUBEX; j++)
		{
			//맨 윗줄
			if (i == 0)
			{
				TCHAR objName[128] = { 0 };
				_stprintf_s(objName, _T("MapCube_%d"), (cubeCnt));
				pGameObject = CCube::Create(m_pGraphicDev ,_vec3{ (float)j * 2,(float)i * 2,10.f });
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(objName, pGameObject), E_FAIL);
				pGameObject->m_pTransform->m_bIsStatic = true;
				cubeCnt++;
			}

			//사이 첫줄
			if (i == CUBEY - 1)
			{
				TCHAR objName[128] = { 0 };
				_stprintf_s(objName, _T("MapCube_%d"), (cubeCnt));
				pGameObject = CCube::Create(m_pGraphicDev, _vec3{ (float)j * 2,(float)i * 2,10.f });
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(objName, pGameObject), E_FAIL);
				pGameObject->m_pTransform->m_bIsStatic = true;
				cubeCnt++;
			}

			//사이 마지막줄
			if (j == 0)
			{
				TCHAR objName[128] = { 0 };
				_stprintf_s(objName, _T("MapCube_%d"), (cubeCnt));
				pGameObject = CCube::Create(m_pGraphicDev, _vec3{ (float)j * 2,(float)i * 2,10.f });
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(objName, pGameObject), E_FAIL);
				pGameObject->m_pTransform->m_bIsStatic = true;
				cubeCnt++;
			}

			//맨 아랫줄
			if (j == CUBEX - 1)
			{
				TCHAR objName[128] = { 0 };
				_stprintf_s(objName, _T("MapCube_%d"), (cubeCnt));
				pGameObject = CCube::Create(m_pGraphicDev, _vec3{ (float)j * 2,(float)i * 2,10.f });
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(objName, pGameObject), E_FAIL);
				pGameObject->m_pTransform->m_bIsStatic = true;
				cubeCnt++;
			}
      
			/*TCHAR objName[128] = {0};
			_stprintf_s(objName, _T("MapCube_%d"), (cubeCnt));
			pGameObject = CCube::Create(m_pGraphicDev);
			pGameObject->m_pTransform->m_vInfo[INFO_POS] = _vec3{ (float)j * 2,(float)i * 2,12.f };
			NULL_CHECK_RETURN(pGameObject, E_FAIL);
			FAILED_CHECK_RETURN(pLayer->Add_GameObject(objName, pGameObject), E_FAIL);
			cubeCnt++;*/
		}
	}

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;


	m_uMapLayer.insert({ pLayerTag, pLayer });
	return S_OK;
}

CStage * CStage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStage *	pInstance = new CStage(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStage::Free(void)
{
	__super::Free();
}