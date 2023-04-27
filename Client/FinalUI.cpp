#include "stdafx.h"
#include "FinalUI.h"
#include"UICamera.h"
#include"Boss2.h"
#include"Boss3.h"
#include"Final3Boss1.h"
#include"Export_Function.h"
#include"AbstractFactory.h"
#include"StageCamera.h"
CFinalUI::CFinalUI(LPDIRECT3DDEVICE9 pGraphicDev) :CGameObject(pGraphicDev)
{
}

CFinalUI::~CFinalUI()
{
}

HRESULT CFinalUI::Ready_GameObject(_float Hp)
{
	
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fMaxHp = Hp;
	m_pTransform->m_vInfo[INFO_POS].y = 350.f;
	m_pTransform->m_vScale = { 600.f,20.f,1.f };
	m_iHp = m_fMaxHp;
	
	m_BarMax = m_pTransform->m_vScale.x;
	return S_OK;
}

_int CFinalUI::Update_GameObject(const _float& fTimeDelta)
{
	

	_vec3 vEye, vAt, vUp;

	vEye = { 0.0f,0.0f,0.0f };
	vAt = { 0.0f,0.0f,1.0f };
	vUp = { 0.0f,1.0f,0.0f };
	
	_float iDam = 1.f;
	m_bDamage = false;
	

	if (GetAsyncKeyState('H'))
		m_bDamage = true;

	
	if (m_bDamage&&m_iHp>0.f)
	{
		
		m_iHp = m_iHp - iDam;
		m_fHpPer = (m_iHp / m_fMaxHp);
		m_pTransform->m_vScale.x = m_BarMax * m_fHpPer;

		m_BarPos = m_BarMax * 1-(iDam/m_fMaxHp);
		m_pTransform->m_vInfo[INFO_POS].x -= iDam*6.f;
		m_bDamage = false;
	}
	
	
	D3DXMatrixOrthoLH(&m_matProjection, WINCX, WINCY, 0, 100.f);
	D3DXMatrixLookAtLH(&m_matViewSpace, &vEye, &vAt, &vUp);
	__super::Update_GameObject(fTimeDelta);
	
	Engine::Add_RenderGroup(RENDER_UI, this);

	return S_OK;
	
}

void CFinalUI::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();

}

void CFinalUI::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matViewSpace);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matProjection);
	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();
	
	__super::Render_GameObject();
}

HRESULT CFinalUI::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });
	
	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"FinalBoss_HP", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"FinalBoss_HP", pComponent });
	

	return S_OK;
}

CFinalUI* CFinalUI::Create(LPDIRECT3DDEVICE9 pGraphicDev, _float Hp)
{
	CFinalUI* pInstance = new CFinalUI(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(Hp)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CFinalUI::Free(void)
{
	__super::Free();
}
