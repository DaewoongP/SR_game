#include "stdafx.h"
#include "BackGround.h"

#include "Export_Function.h"

CBackGround::CBackGround(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CBackGround::~CBackGround()
{
}

HRESULT CBackGround::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale.x = WINCX;
	m_pTransform->m_vScale.y = WINCY;
	m_pTransform->m_vInfo[INFO_POS].x = 0.0f;
	m_pTransform->m_vInfo[INFO_POS].y = 0.0f;
	m_pTransform->m_vInfo[INFO_POS].z = 0.0f;

	return S_OK;
}

_int CBackGround::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_NONE, this);

	return 0;
}
void CBackGround::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CBackGround::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

HRESULT CBackGround::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex",this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });


	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Logo_Texture",this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Logo_Texture", pComponent });

	return S_OK;
}



CBackGround* CBackGround::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CBackGround*		pInstance = new CBackGround(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBackGround::Free(void)
{
	__super::Free();
}
