#include "stdafx.h"
#include "Theme1_Wall.h"


CTheme1_Wall::CTheme1_Wall(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CBackGroundBase(pGraphicDev)
{
}

CTheme1_Wall::~CTheme1_Wall()
{
}

_int CTheme1_Wall::Update_GameObject(const _float & fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

_int CTheme1_Wall::Update_Too(const _float & fTimeDelta)
{
	CTransform* SunTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_Environment", L"T1Sun", L"Transform", ID_DYNAMIC));

	_vec2 sunPos = { SunTransform->m_vInfo[INFO_POS].x, SunTransform->m_vInfo[INFO_POS].y };
	_vec2 ThisPos = { m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y };

	_vec2 vdir = sunPos - ThisPos;

	D3DXVec2Normalize(&vdir, &vdir);

	_vec2 vStandard = { 0.0f,1.0f };

	D3DXVec2Normalize(&vStandard, &vStandard);

	m_pTransform->m_vAngle.z = acosf(D3DXVec2Dot(&vdir, &vStandard));

	if (sunPos.y < ThisPos.y)
		m_pTransform->m_vAngle.z = 2 * D3DX_PI - m_pTransform->m_vAngle.z;

	m_pTransform->Move_Floating(fTimeDelta, 0.01f, 60.f);

	return 0;
}

void CTheme1_Wall::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

CTheme1_Wall * CTheme1_Wall::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos, _float fScale)
{
	CTheme1_Wall*		pInstance = new CTheme1_Wall(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, fScale)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CTheme1_Wall::Ready_GameObject(_vec3 & vPos, _float fScale)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { fScale, fScale, 1.f };

	m_pTransform->m_vInfo[INFO_POS] = vPos;

	_float fAngleInt = rand() % 6;

	m_pTransform->m_fFloating = fAngleInt * 60.f;

	return S_OK;
}

HRESULT CTheme1_Wall::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"T1Wall", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"T1Wall", pComponent });

	return S_OK;
}

void CTheme1_Wall::Free(void)
{
	__super::Free();
}