#include "stdafx.h"
#include "Boss3Eye.h"
#include "Export_Function.h"

CBoss3Eye::CBoss3Eye(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CBoss3Eye::~CBoss3Eye()
{
}

HRESULT CBoss3Eye::Ready_GameObject(_vec3 & vPos, _int iIndex)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = _vec3{vPos.x, vPos.y, vPos.z};
	m_pTransform->m_bIsStatic = true;

	m_iIndex = iIndex;

	m_pParentBoss3 = Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3");

	return S_OK;
}

_int CBoss3Eye::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	_vec3 vPos = m_pParentBoss3->m_pTransform->m_vInfo[INFO_POS];
	m_pTransform->m_vInfo[INFO_POS] = _vec3{ vPos.x - 3.f, vPos.y + 1.f, vPos.z - 4.f };
	
	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

_int CBoss3Eye::Update_Too(const _float & fTimeDelta)
{
	//if (0.f > m_fXAngle)
	//{
	//	m_pTransform->Rotation(ROT_X, D3DXToRadian(-(m_fXAngle)++ * fTimeDelta));
	//}

	CGameObject::Update_Too(fTimeDelta);

	return 0;
}

_int CBoss3Eye::Update_Top(const _float & fTimeDelta)
{
	if (-100.f < m_fXAngle)
		m_pTransform->Rotation(ROT_X, D3DXToRadian(m_fXAngle-- * fTimeDelta));

	CGameObject::Update_Top(fTimeDelta);

	return 0;
}

void CBoss3Eye::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CBoss3Eye::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(m_iIndex);
	m_pBufferCom->Render_Buffer();

	__super::Render_GameObject();
}

HRESULT CBoss3Eye::Add_Component(void)
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

CBoss3Eye * CBoss3Eye::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos, _int iIndex)
{
	CBoss3Eye* pInstance = new CBoss3Eye(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, iIndex)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss3Eye::Free(void)
{
	__super::Free();
}