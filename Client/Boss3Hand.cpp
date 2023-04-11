#include "stdafx.h"
#include "Boss3Hand.h"
#include "Export_Function.h"

CBoss3Hand::CBoss3Hand(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCube(pGraphicDev),
	m_fSpeed(25.f)
{
}

CBoss3Hand::~CBoss3Hand()
{
}

HRESULT CBoss3Hand::Ready_GameObject(_vec3 & vPos, _int iIndex)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vScale = { 2.f, 2.f, 2.f };
	m_pTransform->m_bIsStatic = true;

	m_pCollider->Set_BoundingBox({ 4.f, 4.f, 4.f });
	m_pCollider->Set_Group(COL_OBJ);

	m_iIndex = iIndex;

	return S_OK;
}

_int CBoss3Hand::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

_int CBoss3Hand::Update_Too(const _float & fTimeDelta)
{
	m_pTransform->m_vInfo[INFO_POS].z = 9.f;

	if (0.f > m_fAngle)
		m_pTransform->Rotation(ROT_X, D3DXToRadian(-(m_fAngle)++ * fTimeDelta));

	CGameObject::Update_Too(fTimeDelta);

	return 0;
}

_int CBoss3Hand::Update_Top(const _float & fTimeDelta)
{
	if (-100.f < m_fAngle)
		m_pTransform->Rotation(ROT_X, D3DXToRadian(m_fAngle-- * fTimeDelta));

	FollowPlayer(fTimeDelta);

	CGameObject::Update_Top(fTimeDelta);

	return 0;
}

void CBoss3Hand::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CBoss3Hand::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(m_iIndex);
	m_pBufferCom->Render_Buffer();
	__super::Render_GameObject();
}

void CBoss3Hand::OnCollisionEnter(const Collision * collision)
{
}

void CBoss3Hand::OnCollisionStay(const Collision * collision)
{
}

void CBoss3Hand::OnCollisionExit(const Collision * collision)
{
}

void CBoss3Hand::SwapTrigger()
{
}

HRESULT CBoss3Hand::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"CubeTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"CubeTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Stage3_Boss_Hand_Cube", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Stage3_Boss_Hand_Cube", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Collider", pComponent });

	return S_OK;
}

void CBoss3Hand::FollowPlayer(const _float & fTimeDelta)
{
}

void CBoss3Hand::BossAttack(const _float & fTimeDelta)
{
}

CBoss3Hand * CBoss3Hand::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos, int iIndex)
{
	CBoss3Hand* pInstance = new CBoss3Hand(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, iIndex)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss3Hand::Free(void)
{
	__super::Free();
}
