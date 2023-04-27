#include "stdafx.h"
#include "ProduceToodoo.h"


CProduceToodoo::CProduceToodoo(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CProduceObject(pGraphicDev), m_bPlaySound(false)
{
}

CProduceToodoo::~CProduceToodoo()
{
}

HRESULT CProduceToodoo::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS].x = 1000.0f;
	m_pTransform->m_vInfo[INFO_POS].z = 40.0f;
	m_pTransform->m_vScale = { WINCY*0.5f,WINCY*0.5f,0.0f };


	m_vecLerpList.push_back(_vec3(1000.0f, 0.0f, 0.0f));
	m_vecLerpList.push_back(_vec3(WINCY*0.5f, 0.0f, 1.0f));
	m_vecLerpList.push_back(_vec3(WINCY*0.5f, 0.0f, 1.0f));
	m_vecLerpList.push_back(_vec3(-1000.0f, 0.0f, 1.0f));
	m_vecLerpList.push_back(_vec3(-1000.0f, 0.0f, 1.0f));
	m_vecLerpList.push_back(_vec3(-1000.0f, 0.0f, 1.0f));
	m_vecLerpList.push_back(_vec3(-1000.0f, 0.0f, 1.0f));
	m_vecLerpList.push_back(_vec3(-1000.0f, 0.0f, 1.0f));
	return S_OK;
}

_int CProduceToodoo::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_bStart = dynamic_cast<CProduceObject*>(Engine::Get_GameObject(L"Layer_UI", L"Produce1BackCode"))->Get_Next();

	if (m_bStart)
	{
		Update_Produce(fTimeDelta);
		On_Next(5);
	}

	if (2 == m_iIndex && !m_bPlaySound)
	{
		m_bPlaySound = true;
		StopSound(SOUND_EFFECT);
		PlaySound_Effect(L"34.wav", SOUND_EFFECT, 0.5f);
	}

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CProduceToodoo::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

CProduceToodoo * CProduceToodoo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CProduceToodoo*		pInstance = new CProduceToodoo(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CProduceToodoo::Add_Component()
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"ToodooProduceFace", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"ToodooProduceFace", pComponent });

	return S_OK;
}

void CProduceToodoo::Free()
{
	CGameObject::Free();
}
