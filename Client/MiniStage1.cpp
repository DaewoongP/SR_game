#include "stdafx.h"
#include "MiniStage1.h"
#include "AbstractFactory.h"
#include "Export_Function.h"
#include "Fade.h"
#include "Cube.h"
#include "MoveCamera.h"
#include "DynamicCamera.h"
#include "StageCamera.h"
#include "Topdee.h"
#include "MiniGamePig.h"
#include "FoodCube.h"

CMiniStage1::CMiniStage1(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}


CMiniStage1::~CMiniStage1()
{
}

HRESULT CMiniStage1::Ready_Scene(void)
{
	m_eLoadingID = LOADING_MINI1;
	//m_pFade = CFade::Create(m_pGraphicDev, false);
	FAILED_CHECK_RETURN(Ready_Layer_Environment(L"Layer_Environment"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_GameLogic(L"Layer_GameLogic"), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Layer_UI(L"Layer_UI"), E_FAIL);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	return S_OK;
}

_int CMiniStage1::Update_Scene(const _float & fTimeDelta)
{
	return __super::Update_Scene(fTimeDelta);
}

void CMiniStage1::LateUpdate_Scene(void)
{
	__super::LateUpdate_Scene();
}

void CMiniStage1::Render_Scene(void)
{
}

HRESULT CMiniStage1::Ready_Layer_Environment(const _tchar* pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;

	FAILED_CHECK_RETURN(FACTORY<CStage1Camera>::Create(L"Camera", pLayer), E_FAIL);

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CMiniStage1::Ready_Layer_GameLogic(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;
	FAILED_CHECK_RETURN(FACTORY<CTopdee>::Create(L"Topdee", pLayer, _vec3(14.f, 32.f, 11.f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CMiniGamePig>::Create(L"MiniGamePig", pLayer, _vec3(5.f, 14.f, 8.9f)), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CFoodCube>::Create(L"FoodCube", pLayer, _vec3(10.f, 16.f, 10.f)), E_FAIL);

	FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3(4, 10, 10), 1), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3(4, 12, 10), 1), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3(4, 14, 10), 1), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3(4, 16, 10), 1), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3(4, 18, 10), 1), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3(6, 10, 10), 1), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3(6, 14, 10), 1), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3(6, 12, 10), 1), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3(6, 16, 10), 1), E_FAIL);
	FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3(6, 18, 10), 1), E_FAIL);

	//�ߺ����� �ʴ� ���� ��ġ�� ť����� ��ġ���ٰ�.


	for (int i = 0; i < CUBEY; i++)
	{
		for (int j = 0; j < CUBEX; j++)
		{
			//�� ����
			if (i == 0)
				FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3{ (_float)j * 2,(_float)i * 2,10.f }, 1), E_FAIL);
			//���� ù��
			if (i == CUBEY - 1)
				FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3{ (_float)j * 2,(_float)i * 2,10.f }, 1), E_FAIL);
			//���� ��������
			if (j == 0)
				FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3{ (_float)j * 2,(_float)i * 2,10.f }, 1), E_FAIL);
			//�� �Ʒ���
			if (j == CUBEX - 1)
				FAILED_CHECK_RETURN(FACTORY<CCube>::Create(L"MapCube", pLayer, _vec3{ (_float)j * 2,(_float)i * 2,10.f }, 1), E_FAIL);
		}
	}

	m_uMapLayer.insert({ pLayerTag, pLayer });

	return S_OK;
}

HRESULT CMiniStage1::Ready_Layer_UI(const _tchar * pLayerTag)
{
	CLayer*		pLayer = CLayer::Create();
	NULL_CHECK_RETURN(pLayer, E_FAIL);

	CGameObject*		pGameObject = nullptr;


	m_uMapLayer.insert({ pLayerTag, pLayer });
	return S_OK;
}

CMiniStage1 * CMiniStage1::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMiniStage1 *	pInstance = new CMiniStage1(pGraphicDev);

	if (FAILED(pInstance->Ready_Scene()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMiniStage1::Free(void)
{
	__super::Free();
}