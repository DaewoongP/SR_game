#include "stdafx.h"
#include "Theme4_Chimney.h"


CTheme4_Chimney::CTheme4_Chimney(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CBackGroundBase(pGraphicDev)
{
}

CTheme4_Chimney::~CTheme4_Chimney()
{
}

_int CTheme4_Chimney::Update_GameObject(const _float& fTimeDelta)
{
	m_pTransform->Move_Floating(fTimeDelta, 0.001f*m_pTransform->m_vScale.x, 180.0f, FLOATING_Y);
	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

void CTheme4_Chimney::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

CTheme4_Chimney* CTheme4_Chimney::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _float fScale, _float fAngle)
{
	CTheme4_Chimney* pInstance = new CTheme4_Chimney(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, fScale, fAngle)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CTheme4_Chimney::Ready_GameObject(_vec3& vPos, _float fScale, _float fAngle)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { fScale, fScale, 1.f };

	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vAngle.z = m_fStaticAngle = fAngle;

	return S_OK;
}

HRESULT CTheme4_Chimney::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"T4Chimney", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"T4Chimney", pComponent });

	return S_OK;
}

void CTheme4_Chimney::Free(void)
{
	__super::Free();
}