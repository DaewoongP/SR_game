#include "stdafx.h"
#include "FinalUI_B.h"
#include"UICamera.h"
#include"Final3Boss1.h"
#include"Export_Function.h"
#include"AbstractFactory.h"
#include"StageCamera.h"
CFinalUI_B::CFinalUI_B(LPDIRECT3DDEVICE9 pGraphicDev) :CGameObject(pGraphicDev)
{
}

CFinalUI_B::~CFinalUI_B()
{
}

HRESULT CFinalUI_B::Ready_GameObject(_float Hp)
{

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fMaxHp = Hp;
	m_pTransform->m_vInfo[INFO_POS].y = 350.f;
	m_pTransform->m_vScale = { 620.f,25.f,1.f };
	m_iHp = m_fMaxHp;

	m_BarMax = m_pTransform->m_vScale.x;
	return S_OK;
}

_int CFinalUI_B::Update_GameObject(const _float& fTimeDelta)
{


	_vec3 vEye, vAt, vUp;

	vEye = { 0.0f,0.0f,0.0f };
	vAt = { 0.0f,0.0f,1.0f };
	vUp = { 0.0f,1.0f,0.0f };


	D3DXMatrixOrthoLH(&m_matProjection, WINCX, WINCY, 0, 100.f);
	D3DXMatrixLookAtLH(&m_matViewSpace, &vEye, &vAt, &vUp);
	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_UI, this);

	return S_OK;

}

void CFinalUI_B::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();

}

void CFinalUI_B::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matViewSpace);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProjection);
	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();

	__super::Render_GameObject();
}

HRESULT CFinalUI_B::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });


	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"FinalBoss_HP_B", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"FinalBoss_HP_B", pComponent });


	return S_OK;
}

CFinalUI_B* CFinalUI_B::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float Hp)
{
	CFinalUI_B* pInstance = new CFinalUI_B(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(Hp)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFinalUI_B::Free(void)
{
	__super::Free();
}
