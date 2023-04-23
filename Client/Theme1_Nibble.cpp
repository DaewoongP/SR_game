#include "stdafx.h"
#include "Theme1_Nibble.h"	

CTheme1_Nibble::CTheme1_Nibble(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CBackGroundBase(pGraphicDev)
{
}

CTheme1_Nibble::~CTheme1_Nibble()
{
}

_int CTheme1_Nibble::Update_GameObject(const _float& fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	return 0;
}

_int CTheme1_Nibble::Update_Too(const _float& fTimeDelta)
{

	_vec2 ThisPos = { m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y };



	_vec2 vStandard = { cosf(D3DXToRadian(0.f)),sinf(D3DXToRadian(0.f)) };

	D3DXVec2Normalize(&vStandard, &vStandard);


	m_pTransform->Move_Floating(fTimeDelta, 0.01f, 60.f, FLOATING_Y);

	return 0;
}

void CTheme1_Nibble::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

CTheme1_Nibble* CTheme1_Nibble::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, _float fScale, _float fAngle)
{
	CTheme1_Nibble* pInstance = new CTheme1_Nibble(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, fScale, fAngle)))

	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CTheme1_Nibble::Ready_GameObject(_vec3& vPos, _float fScale, _float fAngle)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { fScale, fScale, 1.f };

	m_pTransform->m_vInfo[INFO_POS] = vPos;

	_float fAngleInt = rand() % 6;

	m_pTransform->m_fFloating = fAngleInt * 60.f;
	m_pTransform->m_vAngle.z = m_fStaticAngle = fAngle;

	return S_OK;
}

HRESULT CTheme1_Nibble::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"T1Nibble", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"T1Nibble", pComponent });

	return S_OK;
}

void CTheme1_Nibble::Free(void)
{
	__super::Free();
}