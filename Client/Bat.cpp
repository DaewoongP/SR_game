#include "stdafx.h"
#include "Bat.h"
#include "Cube.h"
#include "Export_Function.h"
#include "StageCamera.h"
#include "Toodee.h"
#include "Tookee.h"
#include "Topdee.h"
CBat::CBat(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev), m_bMoveLeft(false), m_bBackSprite(false)
{
}

CBat::~CBat()
{
}

HRESULT CBat::Ready_GameObject(_vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fSpeed = 10.0f;
	m_pTransform->m_vScale = { -BATSCALE, BATSCALE, 5.f };
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_bIsStatic = false;
	// 리지드 바디
	m_pRigid->m_bUseGrivaty = false;
	m_pRigid->m_bUseLimitVelocity = true;
	m_pRigid->m_fLimitVelocity = 10.0f;
	m_pRigid->m_bFreezePos_Z = true;
	// 기본 텍스처
	m_pTextureCom->Add_Anim(L"Idle", 0, 6, 0.6f, true);
	m_pTextureCom->Switch_Anim(L"Idle");
	m_pTextureCom->m_bUseFrameAnimation = true;
	// back 텍스처
	m_pTextureCom_Back->Add_Anim(L"Idle", 0, 6, 0.6f, true);
	m_pTextureCom_Back->Switch_Anim(L"Idle");
	m_pTextureCom_Back->m_bUseFrameAnimation = true;

	m_pCollider->Set_Options({ 2.f, 2.f, 4.f }, COL_OBJ, true);

	m_pTransform->m_vInfo[INFO_POS].z = BATTOOZ;

	m_vecCol.push_back(L"MapCube");
	m_vecCol.push_back(L"InstallCube");
	m_vecCol.push_back(L"CrackCube");
	m_vecCol.push_back(L"SwitchCube");
	m_vecCol.push_back(L"MoveCube");
	m_vecCol.push_back(L"GravityCube");
	m_vecCol.push_back(L"KeyCube");
	return S_OK;
}

_int CBat::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	m_pTextureCom->Update_Anim(fTimeDelta);
	__super::Update_GameObject(fTimeDelta);
	
	//X제한
	Value_Range(1.0f, 2.0f * (CUBEX - 1)+1, &m_pTransform->m_vInfo[INFO_POS].x);

	//Y제한
	Value_Range(1.0f, 2.0f * (CUBEY - 1), &m_pTransform->m_vInfo[INFO_POS].y);
	return 0;
}

_int CBat::Update_Too(const _float & fTimeDelta)
{
	m_bBackSprite = false;

	m_pRigid->m_Velocity = { 0.0f, 0.0f, 0.0f };

	_vec3 MoveDir = { 1.0f,0.0f,0.0f };

	if (m_bMoveLeft)
	{
		m_pTransform->m_vScale.x = BATSCALE;
	}
	else if (!m_bMoveLeft)
	{
		m_pTransform->m_vScale.x = -BATSCALE;
		MoveDir *= -1;
	}

	m_pTransform->Move_Pos(&MoveDir, fTimeDelta, m_fSpeed);

	if (m_pTransform->m_vInfo[INFO_POS].z != BATTOOZ)
	{
		m_fTimer += fTimeDelta * 4.0f;

		if (1.0f <= m_fTimer)
		{
			m_fTimer = 1.0f;
		}

		m_pTransform->m_vInfo[INFO_POS].z = Lerp(BATTOPZ, BATTOOZ, m_fTimer);

		if (1.0f == m_fTimer)
		{
			m_fTimer = 0.0f;
		}
	}


	return 0;
}

_int CBat::Update_Top(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	CTransform*	pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Topdee", L"Transform", ID_DYNAMIC));
	if (pPlayerTransformCom == nullptr)return 0;

	_vec3	vPlayerPos;
	pPlayerTransformCom->Get_Info(INFO_POS, &vPlayerPos);

	if (vPlayerPos.y > m_pTransform->m_vInfo[INFO_POS].y)
		m_bBackSprite = true;
	else
		m_bBackSprite = false;

	if (vPlayerPos.x < m_pTransform->m_vInfo[INFO_POS].x)
		m_pTransform->m_vScale.x = -BATSCALE;
	else
		m_pTransform->m_vScale.x = BATSCALE;

	_vec3 vDir;
	D3DXVec3Normalize(&vDir, &(vPlayerPos - m_pTransform->m_vInfo[INFO_POS]));

	m_pRigid->AddForce(vDir, m_fSpeed * 10.f, FORCE, fTimeDelta);
	m_pTextureCom_Back->Update_Anim(fTimeDelta);
	m_bStart2D = true;

	if (m_pTransform->m_vInfo[INFO_POS].z != BATTOPZ)
	{
		m_fTimer += fTimeDelta * 4.0f;

		if (1.0f <= m_fTimer)
		{
			m_fTimer = 1.0f;
		}

		m_pTransform->m_vInfo[INFO_POS].z = Lerp(BATTOOZ, BATTOPZ, m_fTimer);

		if (1.0f == m_fTimer)
		{
			m_fTimer = 0.0f;
		}
	}

	return 0;
}

void CBat::LateUpdate_Too()
{

	if (m_bCol)
	{
		if (m_bMoveLeft)
		{
			m_bMoveLeft = false;
			m_pTransform->m_vScale.x = -BATSCALE;
		}
		else if (!m_bMoveLeft)
		{
			m_bMoveLeft = true;
			m_pTransform->m_vScale.x = BATSCALE;
		}
		m_bCol = false;
	}
}

void CBat::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CBat::Render_GameObject(void)
{
	__super::Render_GameObject();
}

void CBat::SwapTrigger()
{
	if (g_Is2D)
		m_pCollider->Set_BoundingBox({ 2.f, 2.f, 4.f }, { 0.f, 0.f, 0.f });
	else
		m_pCollider->Set_BoundingBox({ 2.f, 2.f, 4.f }, { 0.f, 0.f, 4.f });
}

void CBat::Render_Too()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(0);

	m_pShadowCom->Render_Shadow(m_pBufferCom);

	m_pBufferCom->Render_Buffer();
}

void CBat::Render_Top()
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

void CBat::OnCollisionEnter(const Collision * collision)
{
	TOOKEEDIE;
	TOODEEDIE;
	TOPDEEDIE;

	if ((collision->_dir == DIR_LEFT || collision->_dir == DIR_RIGHT) && (dynamic_cast<CCube*>(collision->otherObj) || !lstrcmp(collision->otherObj->m_pTag, L"Bat")))
	{
		m_bCol = true;
	}

		__super::OnCollisionEnter(collision);
}

void CBat::OnCollisionStay(const Collision * collision)
{
	//if (2.0f > m_pTransform->m_vInfo[INFO_POS].x && !m_bMoveLeft)
	//{m_bMoveLeft = true;
	//	m_pTransform->m_vScale.x = BATSCALE;
	//	
	//	m_pTransform->m_vInfo[INFO_POS].x = 2.0f;
	//}
	//if (2.0f * (CUBEX - 1) < m_pTransform->m_vInfo[INFO_POS].x && m_bMoveLeft)
	//{m_bMoveLeft = false;
	//	m_pTransform->m_vScale.x = -BATSCALE;
	//	
	//	m_pTransform->m_vInfo[INFO_POS].x = 2.0f * (CUBEX - 1);
	//}	
	if (!lstrcmp(collision->otherObj->m_pTag, L"Bat"))
	{
		_vec3 vBoundSize = m_pCollider->Get_BoundSize();

		_vec3 vDir = (m_pTransform->m_vInfo[INFO_POS] - collision->otherObj->m_pTransform->m_vInfo[INFO_POS]);

		vDir.z = 0.0f;
		vBoundSize.z = 0.0f;

		//길이
		_float fLength = (D3DXVec3Length(&vBoundSize) - D3DXVec3Length(&vDir)) * 0.03125f;

		D3DXVec3Normalize(&vDir, &vDir);

		m_pTransform->Move_Pos(&vDir, 1.0f, fLength);
	}
	_float fCameraAngle = dynamic_cast<CStage1Camera*>(Engine::Get_GameObject(L"Layer_Environment", L"Camera"))->m_pTransform->m_vAngle.x;
	if (dynamic_cast<CCube*>(collision->otherObj))
	{
		if (g_Is2D)
		{
			if (m_bStart2D)
			{
				_vec3 dir[8] = { { 1,1,0 },{ 0,1,0 },{ -1,1,0 },{ 1,0,0 },{ 1,-1,0 },{ -1,0,0 },{ -1,-1,0 },{ 0,-1,0 } };

				_vec3 vPosStaticZ = m_pTransform->m_vInfo[INFO_POS];

				vPosStaticZ.z = 10.0f;

				vector<RayCollision> ColCube[8];

				_int SmallIndex = 999;
				_int SmallSize = 999;

				for (size_t i = 0; i < 8; i++)
				{
					ColCube[i] = Engine::Check_Collision_Ray(RAYCAST(vPosStaticZ, dir[i], 4.f),m_pCollider, m_vecCol);

					if (SmallSize > ColCube[i].size())
					{
						SmallSize = ColCube[i].size();
						SmallIndex = i;
					}
				}

				if (SmallIndex%2)
				{
					m_pTransform->Move_Pos(&dir[SmallIndex], 1.0f, 2.0f*ColCube[SmallIndex].size());
				}
				else
					m_pTransform->Move_Pos(&dir[SmallIndex], 1.0f, 2.0f*ColCube[SmallIndex].size()*1.41f);


				m_bStart2D = false;
				
			}
			else
			{
				_vec3 vDir = m_pTransform->m_vInfo[INFO_POS] - collision->otherObj->m_pTransform->m_vInfo[INFO_POS];
				D3DXVec3Normalize(&vDir, &vDir);
				vDir.z = 0.0f;
				m_pTransform->Move_Pos(&vDir, 1.0f, 0.25f);

				if (fCameraAngle && !lstrcmp(L"MapCube", collision->otherObj->m_pTag))
				{
					_vec3 vDir = _vec3(31.f, 17.f, 0.0f) - collision->otherObj->m_pTransform->m_vInfo[INFO_POS];
					D3DXVec3Normalize(&vDir, &vDir);
					vDir.z = 0.0f;
					m_pTransform->Move_Pos(&vDir, 1.0f, 0.25f);
				}
			}
		}
		else if (!g_Is2D)
		{
			m_pShadowCom->m_fShadowHeight = 8.0f;
		}
		
	}

	__super::OnCollisionStay(collision);
}

void CBat::OnCollisionExit(const Collision * collision)
{
	if (dynamic_cast<CCube*>(collision->otherObj))
	{
		m_pShadowCom->m_fShadowHeight = 11.0f;
	}
	__super::OnCollisionExit(collision);
}

HRESULT CBat::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom_Back = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Bat_Back_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom_Back, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Bat_Back_Texture", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Bat_Front_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Bat_Front_Texture", pComponent });

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

CBat * CBat::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos)
{
	CBat*		pInstance = new CBat(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBat::Free(void)
{
	__super::Free();
}
