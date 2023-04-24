#include "stdafx.h"
#include "MiniGamePig.h"

#include "Export_Function.h"

#include "Cube.h"
#include "FoodCube.h"
#include "StageCamera.h"
#include "AbstractFactory.h"
#include "Export_Function.h"

CMiniGamePig::CMiniGamePig(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev),m_bMoveLeft(false), m_bBackSprite(false)
{
}

CMiniGamePig::~CMiniGamePig()
{
}

HRESULT CMiniGamePig::Ready_GameObject(_vec3& vPos)
{
	m_dwDieTimer = 1;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fSpeed = 5.0f;

	m_pCollider->m_bIsTrigger = true;

	m_pRigid->m_bUseGrivaty = false;
	m_pRigid->m_Velocity = _vec3(0, 0, 0);

	m_pTransform->m_vScale = { -MINIPIGSCALE, MINIPIGSCALE, 1.f };
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_bIsStatic = false;

	m_pTextureCom->Add_Anim(L"Idle", 0, 8, 1.f, true);
	m_pTextureCom->Switch_Anim(L"Idle");
	m_pTextureCom->m_bUseFrameAnimation = true;
	
	m_pTextureCom_Back->Add_Anim(L"Idle", 0, 8, 1.f, true);
	m_pTextureCom_Back->Switch_Anim(L"Idle");
	m_pTextureCom_Back->m_bUseFrameAnimation = true;

	m_pCollider->Set_BoundingBox({ 2.f, 2.f, 0.2f });
	m_pCollider->Set_BoundOffset(_vec3(0, 0, 0.1f));
	return S_OK;
}

_int CMiniGamePig::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	m_pTextureCom->Update_Anim(fTimeDelta);
	__super::Update_GameObject(fTimeDelta);
	return 0;
}

_int CMiniGamePig::Update_Too(const _float & fTimeDelta)
{
	m_bBackSprite = false;
	return 0;
}

_int CMiniGamePig::Update_Top(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	m_pRigid->m_bUseGrivaty = false;
	m_pRigid->m_Velocity = _vec3(0, 0, 0);

	m_dwDieTimer -= fTimeDelta;
	if (m_dwDieTimer < 0)
	{
		m_pTransform->m_vInfo[INFO_POS].z += fTimeDelta;
		m_pTransform->m_vAngle.z += D3DXToRadian(10);

		if (m_pTransform->m_vInfo[INFO_POS].z > 11)
		{
			m_bDead = true;
			//자신 새로 생성
			CLayer* pStageLayer = dynamic_cast<CLayer*>(Engine::Get_Layer(L"Layer_GameLogic"));
			NULL_CHECK_RETURN(pStageLayer, E_FAIL);
			FACTORY<CMiniGamePig>::Create(L"MiniGamePig", pStageLayer, _vec3(5.f, 14.f, 8.9f));
		}
	}
		

	m_pTransform->m_vScale.y = MINIPIGSCALE;

	CTransform*	pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"FoodCube", L"Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransformCom, -1);

	_vec3	vPlayerPos;

	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	if (vPlayerPos.y > m_pTransform->m_vInfo[INFO_POS].y)
	{
		m_bBackSprite = true;
	}
	else
		m_bBackSprite = false;
	
	if (vPlayerPos.x < m_pTransform->m_vInfo[INFO_POS].x)
	{
		m_pTransform->m_vScale.x = -MINIPIGSCALE;
	}
	else
		m_pTransform->m_vScale.x = MINIPIGSCALE;

	_vec3	vDir;
	D3DXVec3Normalize(&vDir, &(vPlayerPos - m_pTransform->m_vInfo[INFO_POS]));
	vDir.z = 8.9f;
	_vec3 vdir[MD_END] = { { 0,1,0 },{ 0,-1,0 },{ -1,0,0 },{ 1,0,0 } };
	_vec3 vpos[MD_END] = { 
		{ m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y + 0.6f, m_pTransform->m_vInfo[INFO_POS].z + 2 },
		{ m_pTransform->m_vInfo[INFO_POS].x, m_pTransform->m_vInfo[INFO_POS].y - 0.4f, m_pTransform->m_vInfo[INFO_POS].z + 2 },
		{ m_pTransform->m_vInfo[INFO_POS].x - 0.4f, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vInfo[INFO_POS].z + 2 },
		{ m_pTransform->m_vInfo[INFO_POS].x + 0.6f, m_pTransform->m_vInfo[INFO_POS].y, m_pTransform->m_vInfo[INFO_POS].z + 2 } };
	for (int i = 0; i < 4; i++)
	{
		if (!ShootRay(vdir[i], vpos[i]))
		{
			//미검출 = 이동이 불가능하다.
			switch (i)
			{
			case 0:
				if (vDir.y > 0)
					vDir.y = 0;
				break;
			case 1:
				if (vDir.y < 0)
					vDir.y = 0;
				break;
			case 2:
				if (vDir.x < 0)
					vDir.x = 0;
				break;
			case 3:
				if (vDir.x > 0)
					vDir.x = 0;
				break;
			}
		}
	}
	if (m_IsOnGround)
	{
		m_pTransform->m_vInfo[INFO_POS].x += vDir.x*fTimeDelta*m_fSpeed;
		m_pTransform->m_vInfo[INFO_POS].y += vDir.y*fTimeDelta*m_fSpeed;
	}
	m_pTextureCom_Back->Update_Anim(fTimeDelta);
	return 0;
}

void CMiniGamePig::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CMiniGamePig::Render_GameObject(void)
{
	__super::Render_GameObject();
}

void CMiniGamePig::Render_Too()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pShadowCom->Render_Shadow(m_pBufferCom);

	m_pBufferCom->Render_Buffer();
}

void CMiniGamePig::Render_Top()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	if (m_bBackSprite)
	{
		m_pTextureCom_Back->Set_Texture(0);
	}
	else
		m_pTextureCom->Set_Texture(0);

	m_pShadowCom->Render_Shadow(m_pBufferCom);

	m_pBufferCom->Render_Buffer();
}

void CMiniGamePig::OnCollisionEnter(const Collision * collision)
{
	m_colvec.push_back(collision->otherCol);
	__super::OnCollisionEnter(collision);
	
}

void CMiniGamePig::OnCollisionStay(const Collision * collision)
{
	if (dynamic_cast<CCube*>(collision->otherCol->m_pGameObject))
	{
		m_dwDieTimer = 1;
		m_pTransform->m_vInfo[INFO_POS].z = 8.9f;
		m_pTransform->m_vAngle.z = 0;
		m_IsOnGround = true;
	}
	__super::OnCollisionStay(collision);
}

void CMiniGamePig::OnCollisionExit(const Collision * collision)
{
	auto it = find(m_colvec.begin(), m_colvec.end(), collision->otherCol);
	if (it != m_colvec.end())
		return;
	m_colvec.remove(*it);

	m_IsOnGround = false;
	__super::OnCollisionExit(collision);
}

HRESULT CMiniGamePig::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom_Back = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Back_Pig_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom_Back, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Back_Pig_Texture", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Pig_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Pig_Texture", pComponent });

	pComponent = m_pRigid = dynamic_cast<CRigidbody*>(Engine::Clone_Proto(L"Rigidbody", this));
	NULL_CHECK_RETURN(m_pRigid, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Rigidbody", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });

	pComponent = m_pShadowCom = dynamic_cast<CShadow*>(Engine::Clone_Proto(L"Shadow", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Shadow", pComponent });

	m_pTransform->m_bIsStatic = false;

	return S_OK;
}


CMiniGamePig * CMiniGamePig::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos)
{
	CMiniGamePig*		pInstance = new CMiniGamePig(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

_bool CMiniGamePig::ShootRay(_vec3 dir,_vec3 pos)
{
	//레이의 시작 위치는 목표보다 낮게 설정해줘야함.
	vector<RayCollision> _detectedCOL = Engine::Check_Collision_Ray(RAYCAST(pos, dir, 2.f), m_pCollider);
	if (_detectedCOL.size() !=0)
	{
		for (int i = 0; i < _detectedCOL.size(); i++)
		{
			auto it = find(m_colvec.begin(), m_colvec.end(), _detectedCOL[i].col);
			if(it == m_colvec.end())
				continue;	
			if (dynamic_cast<CCube*>(_detectedCOL[i].col->m_pGameObject))
				return true;
			
		}
			
	}
	return false;
}

void CMiniGamePig::Free(void)
{
	__super::Free();
}
