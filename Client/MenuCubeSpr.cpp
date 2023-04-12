#include "stdafx.h"
#include "MenuCubeSpr.h"


CMenuCubeSpr::CMenuCubeSpr(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CMenuCubeSpr::~CMenuCubeSpr()
{
}

HRESULT CMenuCubeSpr::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	
	m_pTransform->m_vScale = { 100.0f,100.0f,1.0f };

	if (0.0f > m_pTransform->m_vInfo[INFO_POS].x)
	{
		m_pTransform->m_fFloating = 120.0f;
		m_pTransform->m_vScale.x *= -1;
	}

	return S_OK;
}

_int CMenuCubeSpr::Update_GameObject(const _float & fTimeDelta)
{
	m_pTransform->Move_Floating(fTimeDelta, 0.5f ,30.0f);

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CMenuCubeSpr::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

HRESULT CMenuCubeSpr::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"MenuCube_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"MenuCube_Texture", pComponent });

	return S_OK;
}

CMenuCubeSpr * CMenuCubeSpr::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CMenuCubeSpr*		pInstance = new CMenuCubeSpr(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CMenuCubeSpr::Free(void)
{
	__super::Free();
}
