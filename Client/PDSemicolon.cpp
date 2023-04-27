#include "stdafx.h"
#include "PDSemicolon.h"

CPDSemicolon::CPDSemicolon(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CProduceObject(pGraphicDev), iTextureIndex(0), m_bPlaySound(false), m_bPlaySound2(false)
{
}

CPDSemicolon::~CPDSemicolon()
{
}

HRESULT CPDSemicolon::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS].x = 1000.0f;
	m_pTransform->m_vInfo[INFO_POS].z = 40.0f;
	m_pTransform->m_vScale = { WINCY*0.15f,WINCY*0.5f,0.0f };

	m_pTextureCom->Add_Anim(L"Idle", 0, 23, 1.f, true);
	m_pTextureCom->Add_Anim(L"SpeedIdle", 0, 23, 0.1f, true);
	m_pTextureCom->Switch_Anim(L"Idle");
	m_pTextureCom->m_bUseFrameAnimation = true;

	m_vecLerpList.push_back(_vec3(1000.0f, 0.0f, 0.0f));
	m_vecLerpList.push_back(_vec3(0.0f, 1000.0f, 60.0f));
	m_vecLerpList.push_back(_vec3(0.0f, 0.0f, 0.3f));
	m_vecLerpList.push_back(_vec3(0.0f, 0.0f, 0.3f));
	m_vecLerpList.push_back(_vec3(0.0f, 0.0f, 0.5f));
	m_vecLerpList.push_back(_vec3(0.0f, 1000.0f, 60.0f));
	m_vecLerpList.push_back(_vec3(0.0f, 1000.0f, 60.0f));

	return S_OK;
}

_int CPDSemicolon::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_bStart = dynamic_cast<CProduceObject*>(Engine::Get_GameObject(L"Layer_UI", L"PDBackCode"))->Get_Next();

	if (m_bStart)
	{
		m_pTextureCom->Update_Anim(fTimeDelta);
		Update_Produce(fTimeDelta);
		On_Next(5);
	}

	if (!m_bPlaySound && m_iIndex == 2)
	{
		m_bPlaySound = true;
		StopAll();
		PlaySound_Effect(L"33.wav", SOUND_EFFECT_GIMMICK, 0.1f);
		PlaySound_Effect(L"14.wav",SOUND_EFFECT, 1.0f);
	}

	if (!m_bPlaySound2 && 4 == m_iIndex)
	{
		StopSound(SOUND_EFFECT);
		PlaySound_Effect(L"27.wav", SOUND_EFFECT, 0.8f);
		m_bPlaySound2 = true;
		m_bPlaySound = false;
		m_pTextureCom->Switch_Anim(L"SpeedIdle");
	}

	if (!m_bPlaySound && 5 == m_iIndex)
	{
		m_bPlaySound = true;
		StopAll();
		PlaySound_Effect(L"37.wav", SOUND_EFFECT, 0.8f);
	}

	CGameObject::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CPDSemicolon::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(iTextureIndex);

	m_pBufferCom->Render_Buffer();
}

CPDSemicolon * CPDSemicolon::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPDSemicolon*		pInstance = new CPDSemicolon(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CPDSemicolon::Add_Component()
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Semicolon", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Semicolon", pComponent });

	return S_OK;
}

void CPDSemicolon::Free()
{
	__super::Free();
}