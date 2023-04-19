#include "stdafx.h"
#include "Boss3Mouth.h"
#include "Export_Function.h"
#include "AbstractFactory.h"

#include "Fireball.h"

CBoss3Mouth::CBoss3Mouth(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev), m_bShootAnimation(false), m_bShootBullet(false)
{
	m_pBoss3 = nullptr;
}

CBoss3Mouth::~CBoss3Mouth()
{
}

HRESULT CBoss3Mouth::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	
	m_pTransform->m_bIsStatic = true;
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->Rotation(ROT_Y, D3DXToRadian(45.f));
	m_pTextureCom->Add_Anim(L"Idle", 0, 0, 1.f, false);
	m_pTextureCom->Add_Anim(L"Attack", 0, 5, 0.2f, false);
	m_pTextureCom->Switch_Anim(L"Idle");
	m_pTextureCom->m_bUseFrameAnimation = true;

	m_pBoss3 = Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3");

	return S_OK;
}

_int CBoss3Mouth::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	__super::Update_GameObject(fTimeDelta);

	m_pTextureCom->Update_Anim(fTimeDelta);

	if (m_bShootAnimation)
	{
		m_pTextureCom->Switch_Anim(L"Attack");
	}
		
	if (m_pTextureCom->IsAnimationEnd(L"Attack"))
	{
		m_bShootAnimation = false;
		m_pTextureCom->Switch_Anim(L"Idle");
	}

	m_pTransform->Set_ParentTransform(m_pBoss3);

	ShootBullet(fTimeDelta);

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CBoss3Mouth::LateUpdate_GameObject(void)
{
	if (m_bShootAnimation)
	{
		BoundingBox box;
		const _matrix* mat = m_pTransform->Get_WorldMatrixPointer();
		_vec3 vPos = { mat->_41, mat->_42, mat->_43 };
		vPos.y -= 1.f;
		vPos.z -= 2.f;
		box.Offset(vPos);
		m_pFireParticle->Set_BoundingBox(box);
		m_pFireParticle->Set_Size(1.f);
		m_pFireParticle->Set_Options(0.5f);
		m_pFireParticle->Set_RandomGen(1.f);
		m_pFireParticle->Start_Particle();
	}
	__super::LateUpdate_GameObject();
}

void CBoss3Mouth::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();

	__super::Render_GameObject();
	m_pFireParticle->Update_Particle();
}

HRESULT CBoss3Mouth::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Boss3_Mouth_Shoot", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Boss3_Mouth_Shoot", pComponent });

	pComponent = m_pFireParticle = dynamic_cast<CCircularParticle*>(Engine::Clone_Proto(L"FireParticle", this));
	NULL_CHECK_RETURN(m_pFireParticle, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"FireParticle", pComponent });

	return S_OK;
}

void CBoss3Mouth::ShootBullet(const _float & fTimeDelta)
{
	if (m_bShootBullet)
	{
		CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
		NULL_CHECK_RETURN(pStageLayer, );

		_float fX, fY, fZ;
		fX = m_pTransform->Get_WorldMatrixPointer()->m[3][0];
		fY = m_pTransform->Get_WorldMatrixPointer()->m[3][1] - 1.5f;
		fZ = m_pTransform->Get_WorldMatrixPointer()->m[3][2];

		FAILED_CHECK_RETURN(FACTORY<CFireball>::Create(L"Fireball", pStageLayer, _vec3{ fX, fY, fZ}), );

		m_bShootBullet = false;
	}
}

CBoss3Mouth * CBoss3Mouth::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CBoss3Mouth* pInstance = new CBoss3Mouth(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss3Mouth::Free(void)
{
	__super::Free();
}
