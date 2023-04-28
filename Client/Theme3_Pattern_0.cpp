#include "stdafx.h"
#include "Theme3_Pattern_0.h"


CTheme3_Pattern_0::CTheme3_Pattern_0(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CBackGroundBase(pGraphicDev)
{
}

CTheme3_Pattern_0::~CTheme3_Pattern_0()
{
}

_int CTheme3_Pattern_0::Update_GameObject(const _float & fTimeDelta)
{

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

void CTheme3_Pattern_0::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

CTheme3_Pattern_0 * CTheme3_Pattern_0::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos, _float fScale, _float fAngle)
{
	CTheme3_Pattern_0*		pInstance = new CTheme3_Pattern_0(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, fScale, fAngle)))

	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CTheme3_Pattern_0::Ready_GameObject(_vec3 & vPos, _float fScale, _float fAngle)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { fScale, fScale, 1.f };

	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vAngle.z = m_fStaticAngle = fAngle;

	return S_OK;
}

HRESULT CTheme3_Pattern_0::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"T3Pattern_0", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"T3Pattern_0", pComponent });

	return S_OK;
}

void CTheme3_Pattern_0::Free(void)
{
	__super::Free();
}