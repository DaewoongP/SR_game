#include "stdafx.h"
#include "ProduceErroeWindow.h"

CProduceErroeWindow::CProduceErroeWindow(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CProduceObject(pGraphicDev), iTextureIndex(0), m_bPlaySound(false)
{
}

CProduceErroeWindow::~CProduceErroeWindow()
{
}

HRESULT CProduceErroeWindow::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	m_pTransform->m_vScale = { 250.0f*1.0f,176.0*1.0f,0.0f };

	m_eID = Get_Scene()->Get_LoadingID();
	
	m_pTransform->m_vInfo[INFO_POS].x = 1000.0f;
	m_pTransform->m_vInfo[INFO_POS].z = 40.0f;
	
	switch (m_eID)
	{
	case LOADING_PRODUCESCENE1:
	{
		m_vecLerpList.push_back(_vec3(1000.0f, 0.0f, 0.0f));
		m_vecLerpList.push_back(_vec3(0.0f, 0.0f, 60.0f));
		m_vecLerpList.push_back(_vec3(0.0f, 0.0f, 0.5f));
		m_vecLerpList.push_back(_vec3(0.0f, 0.0f, 0.5f));
		m_vecLerpList.push_back(_vec3(0.0f, 0.0f, 0.5f));
	}
	break;
	case LOADING_PRODUCESCENE2:
	{

	}
	break;
	default:
		break;
	}



	return S_OK;
}

_int CProduceErroeWindow::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	switch (m_eID)
	{
	case LOADING_PRODUCESCENE1:
	{
		m_bStart = dynamic_cast<CProduceObject*>(Engine::Get_GameObject(L"Layer_UI", L"ProduceToodooHand"))->Get_Next();

		if (3 == m_iIndex && !m_bPlaySound)
		{
			m_bPlaySound = true;
			StopSound(SOUND_EFFECT);
			PlaySound_Effect(L"13.wav", SOUND_EFFECT, 1.0f);
		}

		if (m_bStart)
		{
			Update_Produce(fTimeDelta);
			On_Next(2);

			if (4 == m_iIndex)
			{
				return STAGE_END;
			}
		}

	}
	break;
	case LOADING_PRODUCESCENE2:
	{
		Update_Produce(fTimeDelta);

	}
	break;
	default:
		break;
	}





	CGameObject::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CProduceErroeWindow::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(iTextureIndex);

	m_pBufferCom->Render_Buffer();
}

CProduceErroeWindow * CProduceErroeWindow::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CProduceErroeWindow*		pInstance = new CProduceErroeWindow(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CProduceErroeWindow::Add_Component()
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Erreo", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Erreo", pComponent });

	return S_OK;
}

void CProduceErroeWindow::Free()
{
	CGameObject::Free();
}