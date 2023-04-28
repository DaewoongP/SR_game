#include "stdafx.h"
#include "Theme4_Gradation.h"


CTheme4_Gradation::CTheme4_Gradation(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CTheme4_Gradation::~CTheme4_Gradation()
{
}

_int CTheme4_Gradation::Update_GameObject(const _float & fTimeDelta)
{

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	return 0;
}

void CTheme4_Gradation::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pBufferGRCom->Render_Buffer();
}

CTheme4_Gradation * CTheme4_Gradation::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos, _float fScale, _float fAngle)
{
	CTheme4_Gradation*		pInstance = new CTheme4_Gradation(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, fScale, fAngle)))

	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CTheme4_Gradation::Ready_GameObject(_vec3 & vPos, _float fScale, _float fAngle)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { fScale, fScale, 1.f };

	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vAngle.z = fAngle;

	return S_OK;
}

HRESULT CTheme4_Gradation::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferGRCom = dynamic_cast<CRcGradation*>(Engine::Clone_Proto(L"RcGradation_4", this));
	NULL_CHECK_RETURN(m_pBufferGRCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcGradation_4", pComponent });

	return S_OK;
}

void CTheme4_Gradation::Free(void)
{
	__super::Free();
}