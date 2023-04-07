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

	//CAMERA
	pGameObject = CDynamiCamera::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Camera", pGameObject), E_FAIL);

	//Key
	pGameObject = CKey::Create(m_pGraphicDev);
	
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Key", pGameObject), E_FAIL);

	//KeyBox
	pGameObject = CKeyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"KeyBox", pGameObject), E_FAIL);
  
	//Pig
	pGameObject = CPig::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Pig_0", pGameObject), E_FAIL);

	//Bat
	pGameObject = CBat::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Bat_0", pGameObject), E_FAIL);
  
	// PLAYER
	pGameObject = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);

	// PLAYER
	pGameObject = CPlayer02::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player02", pGameObject), E_FAIL);

	/*pGameObject = CBat::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Crow", pGameObject), E_FAIL);*/

	pGameObject = CMoveBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoveCube", pGameObject), E_FAIL);
	pGameObject->m_pTransform->m_vInfo[INFO_POS] = _vec3(20, 10, 10);

	pGameObject = CMoveBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"MoveCube", pGameObject), E_FAIL);
	pGameObject->m_pTransform->m_vInfo[INFO_POS] = _vec3(30, 20, 10);

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
				pGameObject = CCube::Create(m_pGraphicDev);
				pGameObject->m_pTransform->m_vInfo[INFO_POS] = _vec3{ (float)j * 2,(float)i * 2,10.f };
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
				pGameObject = CCube::Create(m_pGraphicDev);
				pGameObject->m_pTransform->m_vInfo[INFO_POS] = _vec3{ (float)j * 2,(float)i * 2,10.f };
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
				pGameObject = CCube::Create(m_pGraphicDev);
				pGameObject->m_pTransform->m_vInfo[INFO_POS] = _vec3{ (float)j * 2,(float)i * 2,10.f };
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
				pGameObject = CCube::Create(m_pGraphicDev);
				pGameObject->m_pTransform->m_vInfo[INFO_POS] = _vec3{ (float)j * 2,(float)i * 2,10.f };
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

HRESULT CStage::Ready_LightInfo(void)
{
	D3DLIGHT9		tLightInfo;
	ZeroMemory(&tLightInfo, sizeof(D3DLIGHT9));

	// 점조명, 스포트라이트조명, 방향성조명 중 선택할 수 있다.
	tLightInfo.Type = D3DLIGHT_DIRECTIONAL;
	// 그냥 반사 할 빛
	tLightInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	// 하이라이트 반사할 빛
	tLightInfo.Specular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f);
	// 환경광에 반사할 빛
	tLightInfo.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	//조명의 방향 벡터 : 스포트라이트조명 및 방향성조명의 방향을 준다. 점조명 일때는 의미가 없다.
	tLightInfo.Direction = _vec3(0.0f, 0.0f, 1.0f);

	//조명의 위치 : 점조명 및 스포트라이트조명의 위치를 결정한다. 방향성 조명에는 영향이 없다.
	/*tLightInfo.Position = _vec3(-10.0f, 100.0f, -10.0f);

	tLightInfo.Range = 200.0f;

	tLightInfo.Theta = 0.0f;
	tLightInfo.Phi = D3DXToRadian(180.0f);*/

	FAILED_CHECK_RETURN(Engine::Ready_Light(m_pGraphicDev, &tLightInfo, 0), E_FAIL);

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