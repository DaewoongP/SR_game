#include "stdafx.h"
#include "Lightning.h"

#include"..\Engine\Export_Function.h"
CLightning::CLightning(LPDIRECT3DDEVICE9 pGraphicDev):CGameObject(pGraphicDev)
{
}

CLightning::~CLightning()
{
}


HRESULT CLightning::Ready_GameObject(_vec3& vPos)
{
	m_pTransform->m_vScale = { 1.f,4.f,2.f };

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_vPos = vPos;

	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTextureCom->Add_Anim(L"Idle", 0, 7, 0.5f, false);
	m_pTextureCom->Switch_Anim(L"Idle");
	m_pTextureCom->m_bUseFrameAnimation = true;

	m_pCollider->m_bIsTrigger = true;
	return S_OK;

}

_int CLightning::Update_GameObject(const _float& fTimeDelta)
{
	m_pCollider->Set_BoundingBox({ 1.f,8.f,1.0f });

	float slow = (rand() % 5 - 2);
	m_fLightCycle -= fTimeDelta;
	//_vec3 vPos = m_pTransform->m_vInfo[INFO_POS];
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	m_pTextureCom->Update_Anim(fTimeDelta);
	if (m_pTextureCom->IsAnimationEnd(L"Idle"))
	{
		m_pCollider->Set_BoundingBox({ 0.f,0.f,0.f });
		if (m_fLightCycle<=0)
		{	m_pTextureCom->Switch_Anim(L"Idle");
			m_pTransform->Set_Pos(m_vPos.x + slow,
				m_pTransform->m_vInfo[INFO_POS].y,
				m_pTransform->m_vInfo[INFO_POS].z);
			m_pTextureCom->Set_Loop(L"Idle");
			m_fLightCycle = 1.0f;
			
		}
	}
	//	return OBJ_DEAD;
	__super::Update_GameObject(fTimeDelta);
	return 0;
}

_int CLightning::Update_Too(const _float& fTimeDelta)
{
	__super::Update_Too(fTimeDelta);

	return S_OK;
}

_int CLightning::Update_Top(const _float& fTimeDelta)
{
	__super::Update_Top(fTimeDelta);

	return S_OK;
}

void CLightning::LateUpdate_GameObject(void)
{

	__super::LateUpdate_GameObject();
}

void CLightning::Render_Too()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

void CLightning::Render_Top()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

HRESULT CLightning::Add_Component(void)
{
	CComponent* pComponent = nullptr;
	m_pTransform->m_bIsStatic = false;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Lightning_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Lightning", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });

	return S_OK;
}


CLightning* CLightning::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos)
{
	CLightning* pInstance = new CLightning(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}



void CLightning::Free()
{
	__super::Free();
}
