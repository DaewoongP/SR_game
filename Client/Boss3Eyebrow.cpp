#include "stdafx.h"
#include "Boss3Eyebrow.h"
#include "Export_Function.h"

CBoss3Eyebrow::CBoss3Eyebrow(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
	m_pBoss3 = nullptr;
}

CBoss3Eyebrow::~CBoss3Eyebrow()
{
}

HRESULT CBoss3Eyebrow::Ready_GameObject(_vec3 & vPos, _int iIndex)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = _vec3{ vPos.x, vPos.y, vPos.z };
	m_pTransform->Rotation(ROT_Y, D3DXToRadian(45.f));
	m_pTransform->m_bIsStatic = true;

	m_iIndex = iIndex;

	m_pBoss3 = Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3");
	NULL_CHECK_RETURN(m_pBoss3, E_FAIL);

	return S_OK;
}

_int CBoss3Eyebrow::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	m_pTransform->Set_ParentTransform(m_pBoss3);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CBoss3Eyebrow::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CBoss3Eyebrow::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(m_iIndex);
	m_pBufferCom->Render_Buffer();

	__super::Render_GameObject();
}

HRESULT CBoss3Eyebrow::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Boss3_Eye", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Boss3_Eye", pComponent });

	return S_OK;
}

CBoss3Eyebrow * CBoss3Eyebrow::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos, _int iIndex)
{
	CBoss3Eyebrow* pInstance = new CBoss3Eyebrow(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, iIndex)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss3Eyebrow::Free(void)
{
	__super::Free();
}
