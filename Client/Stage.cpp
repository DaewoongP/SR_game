#include "stdafx.h"
#include "Stage.h"

#include "Export_Function.h"
#include "Monster.h"
#include "Terrain.h"
#include "SkyBox.h"
#include "Cube.h"


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

	//// CDynamicCamera
	/*pGameObject = CDynamicCamera::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"DynamicCamera", pGameObject), E_FAIL);*/
	
	// Terrain
	/*pGameObject = CTerrain::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Terrain", pGameObject), E_FAIL);*/

	pGameObject = CSkyBox::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"SkyBox", pGameObject), E_FAIL);



	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CStage::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{

	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	// PLAYER
	pGameObject = CPlayer::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(pLayer->Add_GameObject(L"Player", pGameObject), E_FAIL);

	int cubeCnt=0;
	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 25; j++)
		{
			//맨 윗줄
			if (i == 0)
			{
				TCHAR objName[128] = { 0 };
				_stprintf_s(objName, _T("Map_Cube_%d"), (cubeCnt));
				pGameObject = CCube::Create(m_pGraphicDev);
				pGameObject->m_pTransform->m_vInfo[INFO_POS] = _vec3{ (float)j*1.99f,(float)i*1.99f,10.f };
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(objName, pGameObject), E_FAIL);
				cubeCnt++;
			}

			//사이 첫줄
			if (i == 19)
			{
				TCHAR objName[128] = { 0 };
				_stprintf_s(objName, _T("Map_Cube_%d"), (cubeCnt));
				pGameObject = CCube::Create(m_pGraphicDev);
				pGameObject->m_pTransform->m_vInfo[INFO_POS] = _vec3{ (float)j*1.99f,(float)i*1.99f,10.f };
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(objName, pGameObject), E_FAIL);
				cubeCnt++;
			}

			//사이 마지막줄
			if (j == 0)
			{
				TCHAR objName[128] = { 0 };
				_stprintf_s(objName, _T("Map_Cube_%d"), (cubeCnt));
				pGameObject = CCube::Create(m_pGraphicDev);
				pGameObject->m_pTransform->m_vInfo[INFO_POS] = _vec3{ (float)j*1.99f,(float)i*1.99f,10.f };
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(objName, pGameObject), E_FAIL);
				cubeCnt++;
			}

			//맨 아랫줄
			if (j == 24)
			{
				TCHAR objName[128] = { 0 };
				_stprintf_s(objName, _T("Map_Cube_%d"), (cubeCnt));
				pGameObject = CCube::Create(m_pGraphicDev);
				pGameObject->m_pTransform->m_vInfo[INFO_POS] = _vec3{ (float)j*1.99f,(float)i*1.99f,10.f };
				NULL_CHECK_RETURN(pGameObject, E_FAIL);
				FAILED_CHECK_RETURN(pLayer->Add_GameObject(objName, pGameObject), E_FAIL);
				cubeCnt++;
			}
			
			/*TCHAR objName[128] = {0};
			_stprintf_s(objName, _T("Map_Cube_%d"), (cubeCnt));
			pGameObject = CCube::Create(m_pGraphicDev);
			pGameObject->m_pTransform->m_vInfo[INFO_POS] = _vec3{(float)j*1.99f,(float)i*1.99f,11.f };
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
