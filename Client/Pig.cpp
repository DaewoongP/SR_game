#include "stdafx.h"
#include "Pig.h"

#include "Export_Function.h"

CPig::CPig(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev)
{
}

CPig::~CPig()
{
}

HRESULT CPig::Ready_GameObject(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fSpeed = 5.0f;
	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };
	m_pTransform->m_vInfo[INFO_POS] = _vec3(10.f, 7.f, 10.f);
	return S_OK;
}

_int CPig::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return _int();
}

void CPig::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CPig::Render_GameObject(void)
{

	__super::Render_GameObject();
}

HRESULT CPig::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Pig_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Pig_Texture", pComponent });
	
	pComponent = m_pRigid = dynamic_cast<CRigidbody*>(Engine::Clone_Proto(L"Rigidbody", this));
	NULL_CHECK_RETURN(m_pRigid, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Rigidbody", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Collider", pComponent });
	m_pCollider->Set_BoundingBox({ 1.f,1.f,0.2f });

	m_pTransform->m_bIsStatic = false;
}

_int CPig::Update_Too(const _float & fTimeDelta)
{
	return 0;
}

_int CPig::Update_Top(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	CTransform*	pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransformCom, -1);

	m_pRigid->m_bUseGrivaty = false;

	_vec3	vPlayerPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	m_pRigid->AddForce(vPlayerPos, m_fSpeed, FORCE, fTimeDelta);
	//여기 들어오는데 트렌스폼 위치가 안바뀜

	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	
	return 0;
}

void CPig::LateUpdate_Too()
{
}

void CPig::LateUpdate_Top()
{
	
}

void CPig::Render_Too()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	__super::Render_GameObject();
}

void CPig::Render_Top()
{

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	__super::Render_GameObject();
}

CPig * CPig::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPig*		pInstance = new CPig(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject()))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPig::Free(void)
{
	__super::Free();
}
