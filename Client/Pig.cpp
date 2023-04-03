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

	return S_OK;
}
_int CPig::Update_GameObject(const _float& fTimeDelta)
{
	CMonster::Update_GameObject(fTimeDelta);
	
	//悼矫 贸府 内靛

	return 0;
}
void CPig::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CPig::Render_GameObject(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pBufferCom->Render_Buffer();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	__super::Render_GameObject();
}

void CPig::OnCollisionEnter(const Collision * collision)
{
}

void CPig::OnCollisionStay(const Collision * collision)
{
}

void CPig::OnCollisionExit(const Collision * collision)
{
}

_int CPig::Updatae_2D(const _float & fTimeDelta)
{
	//2D 捞悼 贸府
	m_pRigid->m_bUseGrivaty = true;

	return 0;
}

_int CPig::Updatae_3D(const _float & fTimeDelta)
{
	//3D 捞悼 贸府
	m_pRigid->m_bUseGrivaty = false;

	CTransform*	pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Player", L"Proto_Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransformCom, -1);

	_vec3	vPlayerPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	m_pTransform->Chase_Target(&vPlayerPos, m_fSpeed, fTimeDelta);
	return 0;
}

HRESULT CPig::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	/*pComponent = m_pBufferCom = dynamic_cast<CTriCol*>(Engine::Clone_Proto(L"TriCol",this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"TriCol", pComponent });

	pComponent = m_pTransform = dynamic_cast<CTransform*>(Engine::Clone_Proto(L"Transform",this));
	NULL_CHECK_RETURN(m_pTransform, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Transform", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Monster_Collider", pComponent });*/
	return S_OK;
}



CPig* CPig::Create(LPDIRECT3DDEVICE9 pGraphicDev)
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