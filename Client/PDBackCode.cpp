#include "stdafx.h"
#include "PDBackCode.h"

CPDBackCode::CPDBackCode(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CProduceObject(pGraphicDev), iTextureIndex(1), m_bPlaySound(false)
{
}

CPDBackCode::~CPDBackCode()
{
}

HRESULT CPDBackCode::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { WINCX*0.4f,WINCY*0.4f,0.0f };

	m_pTransform->m_vInfo[INFO_POS].x = 2000.0f;
	m_pTransform->m_vInfo[INFO_POS].z = 50.0f;

	m_vecLerpList.push_back(_vec3(2000.0f, 0.0f, 0.0f));
	m_vecLerpList.push_back(_vec3(2000.0f, 0.0f, 0.2f));
	m_vecLerpList.push_back(_vec3(2000.0f, 0.0f, 1.0f));
	m_vecLerpList.push_back(_vec3(0.0f, 0.0f, 60.0f));
	m_vecLerpList.push_back(_vec3(0.0f, 0.0f, 0.5f));
	m_vecLerpList.push_back(_vec3(0.0f, 0.0f, 0.5f));//5 출력 
	m_vecLerpList.push_back(_vec3(0.0f, 0.0f, 0.5f));//flase
	m_vecLerpList.push_back(_vec3(0.0f, 0.0f, 0.5f));//7 출력
	m_vecLerpList.push_back(_vec3(0.0f, 0.0f, 0.5f));
	m_vecLerpList.push_back(_vec3(0.0f, 0.0f, 0.1f));



	return S_OK;
}

_int CPDBackCode::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;


	Update_Produce(fTimeDelta);

	On_Next(9);

	m_bStart = dynamic_cast<CProduceObject*>(Engine::Get_GameObject(L"Layer_UI", L"PDSemicolon"))->Get_Next();
	
	if (m_bStart)
	{
		iTextureIndex = 0;
	}

	switch (m_iIndex)
	{
	case 5:
		if (!m_bPlaySound)
		{
			m_bPlaySound = true;
			StopSound(SOUND_EFFECT);
			PlaySound_Effect(L"13.wav", SOUND_EFFECT, 1.0f);
		}
		break;
	case 6:
		m_bPlaySound = false;
		break;
	case 7:
		if (!m_bPlaySound)
		{
			m_bPlaySound = true;
			StopSound(SOUND_EFFECT);
			PlaySound_Effect(L"15.wav", SOUND_EFFECT, 1.0f);
		}
		break;
	default:
		break;
	}


	CGameObject::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CPDBackCode::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(iTextureIndex);

	m_pBufferCom->Render_Buffer();
}

CPDBackCode * CPDBackCode::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPDBackCode*		pInstance = new CPDBackCode(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CPDBackCode::Add_Component()
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"BackCode", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"BackCode", pComponent });

	return S_OK;
}

void CPDBackCode::Free()
{
	__super::Free();
}
