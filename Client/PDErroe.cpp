#include "stdafx.h"
#include "PDErroe.h"


CPDErroe::CPDErroe(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CProduceObject(pGraphicDev), iTextureIndex(0), m_bPlaySound(false)
{
}

CPDErroe::~CPDErroe()
{
}

HRESULT CPDErroe::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { 250.0f*1.0f,176.0*1.0f,0.0f };

	m_pTransform->m_vInfo[INFO_POS].x = 2000.0f;
	m_pTransform->m_vInfo[INFO_POS].z = 40.0f;


	m_vecLerpList.push_back(_vec3(2000.0f, 0.0f, 0.0f));
	m_vecLerpList.push_back(_vec3(2000.0f, 0.0f, 0.2f));
	m_vecLerpList.push_back(_vec3(2000.0f, 0.0f, 1.0f));
	m_vecLerpList.push_back(_vec3(0.0f, 0.0f, 60.0f));
	m_vecLerpList.push_back(_vec3(0.0f, 0.0f, 0.1f));
	m_vecLerpList.push_back(_vec3(0.0f, 0.0f, 0.1f));



	return S_OK;
}

_int CPDErroe::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	
		Update_Produce(fTimeDelta);

		On_Next(5);


		m_bStart = dynamic_cast<CProduceObject*>(Engine::Get_GameObject(L"Layer_UI", L"PDSemicolon"))->Get_Next();

		if (m_bStart)
		{
			m_bDead = true;
		}
		if (!m_bPlaySound && m_iIndex == 4)
		{
			m_bPlaySound = true;
			StopAll();
			PlayBGM(L"52.wav", 0.1f);
		}

	CGameObject::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_UI, this);

	return 0;
}

void CPDErroe::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(iTextureIndex);

	m_pBufferCom->Render_Buffer();
}

CPDErroe * CPDErroe::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPDErroe*		pInstance = new CPDErroe(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CPDErroe::Add_Component()
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

void CPDErroe::Free()
{
	__super::Free();
}