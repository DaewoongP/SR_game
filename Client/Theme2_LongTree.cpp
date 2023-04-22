#include "stdafx.h"
#include "Theme2_LongTree.h"

CTheme2_LongTree::CTheme2_LongTree(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CBackGroundBase(pGraphicDev)
	, m_fMinAngle(0.0f), m_fMaxAngle(0.0f)
{
}

CTheme2_LongTree::~CTheme2_LongTree()
{
}

_int CTheme2_LongTree::Update_GameObject(const _float & fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

_int CTheme2_LongTree::Update_Too(const _float & fTimeDelta)
{
	m_pTransform->Move_Floating(fTimeDelta, 0.f, 180.0f, FLOATING_XY);

	m_fTimer = (sinf(m_pTransform->m_fFloating / 180.f) + 1.0f)*0.5f;

	m_pTransform->m_vAngle.z = Lerp(D3DXToRadian(m_fStaticAngle + m_fMinAngle), D3DXToRadian(m_fStaticAngle + m_fMaxAngle), m_fTimer);

	return 0;
}

void CTheme2_LongTree::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

CTheme2_LongTree * CTheme2_LongTree::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos, _float fScale, _float fAngle)
{
	CTheme2_LongTree*		pInstance = new CTheme2_LongTree(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, fScale, fAngle)))

	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CTheme2_LongTree::Ready_GameObject(_vec3 & vPos, _float fScale, _float fAngle)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { fScale, fScale, 1.f };

	m_pTransform->m_vInfo[INFO_POS] = vPos;

	m_fMinAngle = 10.0f;
	m_fMaxAngle = 11.0f;

	_float fAngleInt = rand() % 6;

	m_pTransform->m_fFloating = fAngleInt * 60.f;

	m_pTransform->m_vAngle.z = m_fStaticAngle = fAngle;


	return S_OK;
}

HRESULT CTheme2_LongTree::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"T2LongTree", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"T2LongTree", pComponent });

	return S_OK;
}

void CTheme2_LongTree::Free(void)
{
	__super::Free();
}