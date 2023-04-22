#include "stdafx.h"
#include "Theme3_Cloud.h"


CTheme3_Cloud::CTheme3_Cloud(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CBackGroundBase(pGraphicDev)
{
}

CTheme3_Cloud::~CTheme3_Cloud()
{
}

_int CTheme3_Cloud::Update_GameObject(const _float & fTimeDelta)
{

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

_int CTheme3_Cloud::Update_Top(const _float & fTimeDelta)
{
	m_pTransform->Move_Pos(&_vec3(1.0f, 0.0f, 0.0f), fTimeDelta, m_fSpeed);
	m_pTransform->Move_Floating(fTimeDelta, 0.01f, 60.0f, FLOATING_Y);
	//만약 위치가 맵 가로 크기 * 1.5f + 본인의 스케일 x 보다커졌다면
	if (CUBEX * 3.0f + m_pTransform->m_vScale.x < m_pTransform->m_vInfo[INFO_POS].x)
	{
		m_pTransform->Set_Pos(
			-(CUBEX + m_pTransform->m_vScale.x),
			m_pTransform->m_vInfo[INFO_POS].y,
			m_pTransform->m_vInfo[INFO_POS].z);
	}
	


	return 0;
}

void CTheme3_Cloud::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

CTheme3_Cloud * CTheme3_Cloud::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos, _float fScale, _float fAngle)
{
	CTheme3_Cloud*		pInstance = new CTheme3_Cloud(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, fScale, fAngle)))

	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CTheme3_Cloud::Ready_GameObject(_vec3 & vPos, _float fScale, _float fAngle)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { fScale, fScale, 1.f };

	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vAngle.z = m_fStaticAngle = fAngle;
	m_fSpeed = 2.0f;
	return S_OK;
}

HRESULT CTheme3_Cloud::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"T3Cloud", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"T3Cloud", pComponent });

	return S_OK;
}

void CTheme3_Cloud::Free(void)
{
	__super::Free();
}