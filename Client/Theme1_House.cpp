#include "stdafx.h"
#include "Theme1_House.h"

CTheme1_House::CTheme1_House(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CBackGroundBase(pGraphicDev)
{
}

CTheme1_House::~CTheme1_House()
{
}

_int CTheme1_House::Update_GameObject(const _float & fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

_int CTheme1_House::Update_Too(const _float & fTimeDelta)
{

	return 0;
}

void CTheme1_House::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

CTheme1_House * CTheme1_House::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos, _float fScale)
{
	CTheme1_House*		pInstance = new CTheme1_House(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, fScale)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CTheme1_House::Ready_GameObject(_vec3 & vPos, _float fScale)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { fScale, fScale, 1.f };

	m_pTransform->m_vInfo[INFO_POS] = vPos;

	return S_OK;
}

HRESULT CTheme1_House::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"T1House", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"T1House", pComponent });

	return S_OK;
}

void CTheme1_House::Free(void)
{
	__super::Free();
}