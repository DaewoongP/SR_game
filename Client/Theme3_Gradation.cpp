#include "stdafx.h"
#include "Theme3_Gradation.h"


CTheme3_Gradation::CTheme3_Gradation(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{
}

CTheme3_Gradation::~CTheme3_Gradation()
{
}

_int CTheme3_Gradation::Update_GameObject(const _float & fTimeDelta)
{

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	return 0;
}

void CTheme3_Gradation::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pBufferGRCom->Render_Buffer();
}

CTheme3_Gradation * CTheme3_Gradation::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos, _float fScale, _float fAngle)
{
	CTheme3_Gradation*		pInstance = new CTheme3_Gradation(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, fScale, fAngle)))

	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CTheme3_Gradation::Ready_GameObject(_vec3 & vPos, _float fScale, _float fAngle)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { fScale, fScale, 1.f };

	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vAngle.z = fAngle;

	return S_OK;
}

HRESULT CTheme3_Gradation::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferGRCom = dynamic_cast<CRcGradation*>(Engine::Clone_Proto(L"RcGradation_3", this));
	NULL_CHECK_RETURN(m_pBufferGRCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcGradation_3", pComponent });

	return S_OK;
}

void CTheme3_Gradation::Free(void)
{
	__super::Free();
}