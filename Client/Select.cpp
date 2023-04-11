#include "stdafx.h"
#include "Select.h"
#include"Logo.h"
#include"..\Engine\Export_Function.h"
CSelect::CSelect(LPDIRECT3DDEVICE9 pGraphicDev):CGameObject(pGraphicDev)
{
}

CSelect::~CSelect()
{
}

HRESULT CSelect::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { 100.f,80.0f,1.0f };

	m_pTransform->m_vInfo[INFO_POS].y=-50.0f;
	m_pTextureCom->Add_Anim(L"Start", 9, 9, 1.f, false);
	m_pTextureCom->Add_Anim(L"Exit", 10, 10, 1.f, false);

	m_pTextureCom->Switch_Anim(L"Start");

	return S_OK;
}

_int CSelect::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	__super::Update_GameObject(fTimeDelta);


	return 0;
}

void CSelect::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	if (CLogo::Get_Start()==true)
		m_pTextureCom->Set_Texture(9);
	else if (CLogo::Get_Start() == false)
		m_pTextureCom->Set_Texture(10);


	m_pBufferCom->Render_Buffer();
}

HRESULT CSelect::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Select_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Texture", pComponent });

	return S_OK;
}

CSelect* CSelect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSelect* pInstance = new CSelect(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CSelect::Free(void)
{
	__super::Free();
}
