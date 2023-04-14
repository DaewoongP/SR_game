#include"stdafx.h"
#include "LightningCloud.h"

#include"..\Engine\Export_Function.h"
#include "AbstractFactory.h"
CLightningCloud::CLightningCloud(LPDIRECT3DDEVICE9 pGraphicDev):CCloud(pGraphicDev)
{
}

CLightningCloud::~CLightningCloud()
{
}

HRESULT CLightningCloud::Ready_GameObject(_vec3& vPos)
{
	m_pTransform->m_vScale = { 4.f,4.f,2.f };

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTextureCom->Add_Anim(L"Idle", 0, 5, 1.f, true);
	m_pTextureCom->Switch_Anim(L"Idle");
	m_pTextureCom->m_bUseFrameAnimation = true;

	m_pCollider->Set_BoundingBox({ m_pTransform->m_vScale.x + 2,2.f,1.0f });
	m_pCollider->m_bIsTrigger = true;
	

	return S_OK;
}

_int CLightningCloud::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	m_pTextureCom->Update_Anim(fTimeDelta);
	__super::Update_GameObject(fTimeDelta);
	return 0;
}

_int CLightningCloud::Update_Too(const _float& fTimeDelta)
{
	__super::Update_Too(fTimeDelta);

	return S_OK;
}

_int CLightningCloud::Update_Top(const _float& fTimeDelta)
{
	__super::Update_Top(fTimeDelta);

	return S_OK;
}

void CLightningCloud::Render_Too()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());


	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

void CLightningCloud::Render_Top()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
}

HRESULT CLightningCloud::Add_Component(void)
{
	CComponent* pComponent = nullptr;
	m_pTransform->m_bIsStatic = false;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Cloud_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Texture", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });
	return S_OK;
}

CLightningCloud* CLightningCloud::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos, CLayer* pLayer)
{
	CLightningCloud* pInstance = new CLightningCloud(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	FAILED_CHECK_RETURN(FACTORY<CLightning>::Create(L"Lightning", pLayer, _vec3(30.f, 10.f, 10.f)), nullptr);

	return pInstance;
}

void CLightningCloud::Free(void)
{
	__super::Free();
}
