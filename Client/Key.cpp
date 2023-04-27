#include "stdafx.h"
#include "Key.h"

#include"Export_Function.h"

size_t CKey::iKeyCnt = 0;

CKey::CKey(LPDIRECT3DDEVICE9 pGraphicDev) 
	:CGameObject(pGraphicDev)
{
}

CKey::~CKey()
{
}

HRESULT CKey::Ready_GameObject(_vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vScale = { 1.f,1.f,1.f };
	m_pTransform->m_bIsStatic = false;

	m_pTextureCom->Add_Anim(L"Idle", 0, 21, 1.f, true);
	m_pTextureCom->Switch_Anim(L"Idle");
	m_pTextureCom->m_bUseFrameAnimation = true;

	m_pCollider->Set_Options({ 1.f, 1.f, 2.f }, COL_ENV, true);
	
	BoundingBox box;
	box.Offset(vPos);
	m_pCircularParticle->Set_BoundingBox(box);
	m_pCircularParticle->Set_Size(0.7f);
	m_pCircularParticle->Set_Options(0.2f, 10.f);
	box._offsetMin = { -0.2f, -0.2f, 0.f };
	box._offsetMax = { 0.2f, 0.2f, 0.f };
	m_pSparkParticle->Set_BoundingBox(box);
	m_pSparkParticle->Set_AnimSpeed(0.05f);
	m_pSparkParticle->Set_LifeTime(0.f);
	m_pSparkParticle->Start_Particle();

	++iKeyCnt;
	return S_OK;
}

_int CKey::Update_GameObject(const _float& fTimeDelta)
{	
	if (m_pCircularParticle->IsDead())
		return OBJ_DEAD;

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	__super::Update_GameObject(fTimeDelta);
	m_pTextureCom->Update_Anim(fTimeDelta);
	return 0;
}

void CKey::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CKey::Render_GameObject(void)
{
	if (-1 == m_pCircularParticle->Update_Particle())
		return; // 렌더링을 먼저 없애기 위한 용도
	m_pSparkParticle->Update_Particle();

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pShadow->Render_Shadow(m_pBufferCom);

	m_pBufferCom->Render_Buffer();

	__super::Render_GameObject();
}

void CKey::OnCollisionEnter(const Collision* collision)
{	
	if (!lstrcmp(collision->otherObj->m_pTag, L"Boss1Head"))
		return;
	if (iKeyCnt > 0)
	{
		StopSound(SOUND_EFFECT_GIMMICK);
		PlaySound_Effect(L"56.wav", SOUND_EFFECT_GIMMICK, 1.f);
		--iKeyCnt;
		m_bDead = true;
	}
	m_pCircularParticle->Start_Particle();
	__super::OnCollisionEnter(collision);
}

void CKey::OnCollisionExit(const Collision * collision)
{
	__super::OnCollisionExit(collision);
}

HRESULT CKey::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex" , pComponent});

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Key_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Texture",pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider",pComponent });

	pComponent = m_pSparkParticle = dynamic_cast<CSparkParticle*>(Engine::Clone_Proto(L"SparkParticle", this));
	NULL_CHECK_RETURN(m_pSparkParticle, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"SparkParticle", pComponent });

	pComponent = m_pCircularParticle = dynamic_cast<CCircularParticle*>(Engine::Clone_Proto(L"CircularParticle", this));
	NULL_CHECK_RETURN(m_pCircularParticle, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"CircularParticle", pComponent });

	pComponent = m_pShadow = dynamic_cast<CShadow*>(Engine::Clone_Proto(L"Shadow", this));
	NULL_CHECK_RETURN(m_pShadow, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Shadow", pComponent });

	return S_OK;
}

CKey* CKey::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos)
{  	
	CKey* pInstance = new CKey(pGraphicDev);
	
	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CKey::Free(void)
{
	__super::Free();
}
