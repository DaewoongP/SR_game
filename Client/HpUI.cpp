#include "stdafx.h"
#include "HpUI.h"
#include"UICamera.h"
#include"Boss2.h"
#include"Boss3.h"
#include"Export_Function.h"
#include"AbstractFactory.h"
#include"StageCamera.h"
CHpUI::CHpUI(LPDIRECT3DDEVICE9 pGraphicDev) :CGameObject(pGraphicDev)
{
}

CHpUI::~CHpUI()
{
}

HRESULT CHpUI::Ready_GameObject(_int Hp,_int i)
{
	m_iHp = Hp;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = _vec3{ 500.f+i*40,-450.f,1.f };
	m_pTransform->m_vScale = { 20,20,1.f };
	D3DXMatrixOrthoLH(&m_matProjection, WINCX, WINCY, 0, 100.f);



	_vec3 vEye, vAt, vUp;

	vEye = { 0.0f,0.0f,0.0f };
	vAt = { 0.0f,0.0f,1.0f };
	vUp = { 0.0f,1.0f,0.0f };

	D3DXMatrixLookAtLH(&m_matViewSpace, &vEye, &vAt, &vUp);
	return S_OK;
}

_int CHpUI::Update_GameObject(const _float& fTimeDelta)
{
	m_HPUI += fTimeDelta;
	if(m_pTransform->m_vInfo[INFO_POS].y<=-360&&m_HPUI>=1.f)
	m_pTransform->m_vInfo[INFO_POS].y += 10.f;
	
	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_UI, this);

	return S_OK;

}

void CHpUI::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();

}

void CHpUI::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matViewSpace);

	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProjection);
	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
	__super::Render_GameObject();
}

HRESULT CHpUI::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });


	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Boss_HP_Tex", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Boss_HP_Tex", pComponent });

	return S_OK;
}

CHpUI* CHpUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, _int Hp,_int i)
{
	CHpUI* pInstance = new CHpUI(pGraphicDev);
	
		if (FAILED(pInstance->Ready_GameObject(Hp,i)))
		{
			Safe_Release(pInstance);
			return nullptr;
		}
	
	return pInstance;
}

void CHpUI::Free(void)
{
	__super::Free();
}
