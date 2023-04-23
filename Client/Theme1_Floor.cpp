#include "stdafx.h"
#include "Theme1_Floor.h"

CTheme1_Floor::CTheme1_Floor(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CBackGroundBase(pGraphicDev)
{
}

CTheme1_Floor::~CTheme1_Floor()
{
}

_int CTheme1_Floor::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

void CTheme1_Floor::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(m_iTexNum);

	m_pBufferCom->Render_Buffer();

}

CTheme1_Floor* CTheme1_Floor::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _float fScale, _int TexNum)
{
	CTheme1_Floor* pInstance = new CTheme1_Floor(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, fScale, TexNum)))

	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CTheme1_Floor::Ready_GameObject(_vec3& vPos, _float fScale, _int TexNum)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { fScale, fScale, 1.f };

	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_iTexNum = TexNum;
	return S_OK;
}

HRESULT CTheme1_Floor::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"T1Floor", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"T1Floor", pComponent });

	return S_OK;
}

void CTheme1_Floor::Free(void)
{
	__super::Free();
}