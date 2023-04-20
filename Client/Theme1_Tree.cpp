#include "stdafx.h"
#include "Theme1_Tree.h"

CTheme1_Tree::CTheme1_Tree(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CBackGroundBase(pGraphicDev)
{
}

CTheme1_Tree::~CTheme1_Tree()
{
}

_int CTheme1_Tree::Update_GameObject(const _float & fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

_int CTheme1_Tree::Update_Too(const _float & fTimeDelta)
{
	CTransform* SunTransform = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_Environment", L"T1Sun", L"Transform", ID_DYNAMIC));

	_vec2 sunPos = { SunTransform->m_vInfo[INFO_POS].x, SunTransform->m_vInfo[INFO_POS].y };
	_vec2 ThisPos = { m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y };
	
	_vec2 vdir = sunPos - ThisPos;

	D3DXVec2Normalize(&vdir, &vdir);

	m_pTransform->m_vAngle.z = acosf(D3DXVec2Dot(&vdir, &_vec2(1.0f, 0.0f)));
	
	if (sunPos.y < ThisPos.y)
		m_pTransform->m_vAngle.z = 2 * D3DX_PI - m_pTransform->m_vAngle.z;

	m_pTransform->Move_Floating(fTimeDelta, 0.01f, 60.f);

	return 0;
}

void CTheme1_Tree::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
	__super::Render_GameObject();
}

CTheme1_Tree * CTheme1_Tree::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos, _float fScale)
{
	CTheme1_Tree*		pInstance = new CTheme1_Tree(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, fScale)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CTheme1_Tree::Ready_GameObject(_vec3 & vPos, _float fScale)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { fScale, fScale, 1.f };

	m_pTransform->m_vInfo[INFO_POS] = vPos;

	_float fAngleInt = rand() % 6;

	m_pTransform->m_fFloating = fAngleInt * 60.f;

	return S_OK;
}

HRESULT CTheme1_Tree::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"T1Tree", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"T1Tree", pComponent });

	return S_OK;
}

void CTheme1_Tree::Free(void)
{
	__super::Free();
}