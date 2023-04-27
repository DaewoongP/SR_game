#include "stdafx.h"
#include "ProduceToodooHand.h"

CProduceToodooHand::CProduceToodooHand(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CProduceObject(pGraphicDev), iTextureIndex(0), m_bPlaySound(false)
{
}

CProduceToodooHand::~CProduceToodooHand()
{
}

HRESULT CProduceToodooHand::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS].x = 1000.0f;
	m_pTransform->m_vInfo[INFO_POS].z = 40.0f;
	m_pTransform->m_vScale = { WINCY*0.5f,WINCY*0.5f,0.0f };

	m_vecLerpList.push_back(_vec3(1000.0f, 0.0f, 0.0f));
	m_vecLerpList.push_back(_vec3(0.0f, 0.0f, 1.0f));
	m_vecLerpList.push_back(_vec3(0.0f, 0.0f, 1.0f));
	m_vecLerpList.push_back(_vec3(1000.0f, 0.0f, 1.0f));

	return S_OK;
}

_int CProduceToodooHand::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_bStart = dynamic_cast<CProduceObject*>(Engine::Get_GameObject(L"Layer_UI", L"ProduceToodoo"))->Get_Next();

	if (m_bStart)
	{
		Update_Produce(fTimeDelta);
		On_Next(4);

	}

	if (3 == m_iIndex && !m_bPlaySound)
	{
		iTextureIndex = 1;
		m_bPlaySound = true;
		StopSound(SOUND_EFFECT);
		PlaySound_Effect(L"4.wav", SOUND_EFFECT, 0.2f);
	}


	CGameObject::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CProduceToodooHand::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(iTextureIndex);

	m_pBufferCom->Render_Buffer();
}

CProduceToodooHand * CProduceToodooHand::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CProduceToodooHand*		pInstance = new CProduceToodooHand(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CProduceToodooHand::Add_Component()
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"ToodooProduceHand", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"ToodooProduceHand", pComponent });

	return S_OK;
}

void CProduceToodooHand::Free()
{
	CGameObject::Free();
}