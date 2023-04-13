#include "stdafx.h"
#include "Boss2Hand.h"
#include "Export_Function.h"
#include "StageCamera.h"
#include "Cube.h"

CBoss2Hand::CBoss2Hand(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}


CBoss2Hand::~CBoss2Hand()
{
}

HRESULT CBoss2Hand::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	//지정된 위쪽에서 태어나겠지
	m_pTransform->m_vScale = _vec3(7, 14, 5);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_bIsStatic = false;

	m_pCollider->Set_BoundingBox(_vec3(9,18,2));

	m_pRigid->m_bUseGrivaty = false;

	return S_OK;
}

_int CBoss2Hand::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return -1;
	__super::Update_GameObject(fTimeDelta);
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	//DoActionSwitch(fTimeDelta);
	return 0;
}

_int CBoss2Hand::Update_Too(const _float & fTimeDelta)
{
	return _int();
}

_int CBoss2Hand::Update_Top(const _float & fTimeDelta)
{
	return _int();
}

void CBoss2Hand::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CBoss2Hand::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();
	__super::Render_GameObject();
}

void CBoss2Hand::OnCollisionEnter(const Collision * collision)
{
}

void CBoss2Hand::OnCollisionStay(const Collision * collision)
{
}

void CBoss2Hand::OnCollisionExit(const Collision * collision)
{
}

void CBoss2Hand::SwapTrigger()
{
}

HRESULT CBoss2Hand::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Boss2_Hand", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Texture", pComponent });

	pComponent = m_pRigid = dynamic_cast<CRigidbody*>(Engine::Clone_Proto(L"Rigidbody", this));
	NULL_CHECK_RETURN(m_pRigid, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Rigidbody", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });

	return S_OK;
}

void CBoss2Hand::DoActionSwitch(const _float & fTimeDelta)
{
	switch (m_state)
	{
	case B2H_APPEAR:
		D0_Appear(fTimeDelta);
		break;
	case B2H_LITTLEUP:
		D0_LittleUp(fTimeDelta);
		break;
	case B2H_DROP:
		D0_Drop(fTimeDelta);
		break;
	case B2H_DISAPPEAR:
		D0_DisAppear(fTimeDelta);
		break;
	case B2H_DIE:
		break;
	}
}

void CBoss2Hand::D0_Appear(const _float & fTimeDelta)
{
	if (m_dwActionTimer < 0)
	{
		//m_pRigid->
	}
}

void CBoss2Hand::D0_LittleUp(const _float & fTimeDelta)
{
}

void CBoss2Hand::D0_Drop(const _float & fTimeDelta)
{
}

void CBoss2Hand::D0_DisAppear(const _float & fTimeDelta)
{
}

CBoss2Hand * CBoss2Hand::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CBoss2Hand*		pInstance = new CBoss2Hand(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss2Hand::Free(void)
{
	__super::Free();
}
