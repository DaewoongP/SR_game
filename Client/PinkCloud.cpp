#include "stdafx.h"
#include "PinkCloud.h"

#include "Export_Function.h"

CPinkCloud::CPinkCloud(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CCloud(pGraphicDev)
{
}

CPinkCloud::~CPinkCloud()
{
}

HRESULT CPinkCloud::Ready_GameObject(_vec3 & vPos)
{
	m_pTransform->m_vScale = { 4.f, 4.f, 2.0f };

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTextureCom->Add_Anim(L"Idle", 0, 5, 1.f, true);
	m_pTextureCom->Switch_Anim(L"Idle");
	m_pTextureCom->m_bUseFrameAnimation = true;

	m_pCollider->Set_BoundingBox({ m_pTransform->m_vScale.x + 2,2.f,1.0f });
	m_pCollider->m_bIsTrigger = true;
	
	return S_OK;
}

_int CPinkCloud::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	m_pTextureCom->Update_Anim(fTimeDelta);
	__super::Update_GameObject(fTimeDelta);
	return 0;
}

_int CPinkCloud::Update_Too(const _float & fTimeDelta)
{
	return S_OK;
}

_int CPinkCloud::Update_Top(const _float & fTimeDelta)
{
	return S_OK;
}

void CPinkCloud::Render_Too()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pShadow->Render_Shadow(m_pBufferCom);

	m_pBufferCom->Render_Buffer();
}

void CPinkCloud::Render_Top()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pShadow->Render_Shadow(m_pBufferCom);

	m_pBufferCom->Render_Buffer();
}

HRESULT CPinkCloud::Add_Component(void)
{
	CComponent*		pComponent = nullptr;
	m_pTransform->m_bIsStatic = false;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Cloud_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Texture", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Collider", pComponent });
	m_pCollider->Set_BoundingBox({ m_pTransform->m_vScale.x+2,2.f,1.0f });

	pComponent = m_pShadow = dynamic_cast<CShadow*>(Engine::Clone_Proto(L"Shadow", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Shadow", pComponent });
	return S_OK;
}

CPinkCloud * CPinkCloud::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CPinkCloud*		pInstance = new CPinkCloud(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPinkCloud::Free(void)
{
	__super::Free();
}
