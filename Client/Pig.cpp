#include "stdafx.h"
#include "Pig.h"

#include "Export_Function.h"

#include "Cube.h"

#include "StageCamera.h"

CPig::CPig(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMonster(pGraphicDev),m_bMoveLeft(false), m_bBackSprite(false)
{
}

CPig::~CPig()
{
}

HRESULT CPig::Ready_GameObject(_vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_fSpeed = 8.0f;
	m_pTransform->m_vScale = { -PIGSCALE, PIGSCALE, 1.f };
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_bIsStatic = false;

	m_pTextureCom->Add_Anim(L"Idle", 0, 8, 1.f, true);
	m_pTextureCom->Switch_Anim(L"Idle");
	m_pTextureCom->m_bUseFrameAnimation = true;
	
	m_pTextureCom_Back->Add_Anim(L"Idle", 0, 8, 1.f, true);
	m_pTextureCom_Back->Switch_Anim(L"Idle");
	m_pTextureCom_Back->m_bUseFrameAnimation = true;

	m_pCollider->Set_BoundingBox({ 2.f, 2.f, 0.2f }, { 0.f, -1.f, 0.f });
	
	return S_OK;
}

_int CPig::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	m_pTextureCom->Update_Anim(fTimeDelta);
	__super::Update_GameObject(fTimeDelta);
	return 0;
}

_int CPig::Update_Too(const _float & fTimeDelta)
{
	m_pTransform->m_vAngle.z = 0.0f;
	m_pTransform->m_vInfo[INFO_POS].z = 10.f;
	m_bBackSprite = false;

	_vec3 MoveDir = { 1.0f,0.0f,0.0f };

	m_pRigid->m_bUseGrivaty = true;

	if (-1.0f > m_pRigid->m_Velocity.y)
	{
		if (-5.0f > m_pRigid->m_Velocity.y)
		{
			_float m_fFallingScale = (fabsf(m_pRigid->m_Velocity.y + 4.0f));

			if (-9.0f > m_pRigid->m_Velocity.y)
			{
				m_fFallingScale = 5.0f;
			}

			if (m_bMoveLeft)
			{
				m_pTransform->m_vScale.x = PIGSCALE / m_fFallingScale*2.0f;
			}
			else
				m_pTransform->m_vScale.x = -PIGSCALE / m_fFallingScale*2.0f;
			
			m_pTransform->m_vScale.y = PIGSCALE + PIGSCALE - PIGSCALE / m_fFallingScale;
		}
	}
	else
	{
		if (m_pTransform->m_vScale.y > PIGSCALE)
		{
			m_pTransform->m_vScale.y -= PIGSCALE * 0.1f;
		}
		else
			m_pTransform->m_vScale.y = PIGSCALE;

		if (m_bMoveLeft)
		{
			m_pTransform->m_vScale.x = PIGSCALE;
		}
		else if (!m_bMoveLeft)
		{
			m_pTransform->m_vScale.x = -PIGSCALE;
			MoveDir *= -1;
		}

		m_pTransform->Move_Pos(&MoveDir, fTimeDelta, m_fSpeed);
	}
	return 0;
}

_int CPig::Update_Top(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_pTransform->m_vInfo[INFO_POS].z = 10.0f;

	m_pTransform->m_vScale.y = PIGSCALE;

	CTransform*	pPlayerTransformCom = dynamic_cast<CTransform*>(Engine::Get_Component(L"Layer_GameLogic", L"Topdee", L"Transform", ID_DYNAMIC));
	NULL_CHECK_RETURN(pPlayerTransformCom, -1);

	m_pRigid->m_Velocity = { 0.0f, 0.0f, 0.0f };

	m_pRigid->m_bUseGrivaty = false;

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
		m_pTransform->m_vScale.x = -PIGSCALE;
	}
	else
		m_pTransform->m_vScale.x = PIGSCALE;

	_vec3	vDir;
	D3DXVec3Normalize(&vDir, &(vPlayerPos - m_pTransform->m_vInfo[INFO_POS]));

	m_pTransform->Move_Pos(&vDir, fTimeDelta, m_fSpeed);
	m_pTextureCom_Back->Update_Anim(fTimeDelta);
	return 0;
}

void CPig::LateUpdate_Too()
{
	if (m_bCol)
	{
		if (m_bMoveLeft)
		{
			m_bMoveLeft = false;
			m_pTransform->m_vScale.x = -PIGSCALE;
		}
		else if (!m_bMoveLeft)
		{
			m_bMoveLeft = true;
			m_pTransform->m_vScale.x = PIGSCALE;
		}
		m_bCol = false;
	}
}

void CPig::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CPig::Render_GameObject(void)
{
	__super::Render_GameObject();
}

void CPig::Render_Too()
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pShadowCom->Render_Shadow(m_pBufferCom);

	m_pBufferCom->Render_Buffer();
}

void CPig::Render_Top()
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

void CPig::OnCollisionEnter(const Collision * collision)
{
	if ((collision->_dir == DIR_LEFT || collision->_dir == DIR_RIGHT) && (dynamic_cast<CCube*>(collision->otherObj) || !lstrcmp(collision->otherObj->m_pTag, L"Pig")))
	{
		//카메라 쉐이크 사용함수 예
		//dynamic_cast<CStage1Camera*>(Engine::Get_GameObject(L"Layer_Environment", L"Camera"))->Start_Camera_Shake(1.0f, 100.0f, SHAKE_ALL);

		/*if (m_bMoveLeft)
		{
			m_bMoveLeft = false;
			m_pTransform->m_vScale.x = -PIGSCALE;
		}
		else if (!m_bMoveLeft)
		{
			m_bMoveLeft = true;
			m_pTransform->m_vScale.x = PIGSCALE;
		}*/
		m_bCol = true;
	}
	if (!lstrcmp(collision->otherObj->m_pTag, L"Spike")&&( collision->_dir == DIR_DOWN || collision->_dir == DIR_LEFT || collision->_dir == DIR_RIGHT))
		m_bDead = true;
	
	__super::OnCollisionEnter(collision);
}

void CPig::OnCollisionStay(const Collision * collision)
{
	if (!lstrcmp(collision->otherObj->m_pTag, L"Pig"))
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
	if (!lstrcmp(collision->otherObj->m_pTag, L"Spike"))
	{
		m_bDead = true;
	}

	__super::OnCollisionStay(collision);
}

void CPig::OnCollisionExit(const Collision * collision)
{
	__super::OnCollisionExit(collision);
}

HRESULT CPig::Add_Component(void)
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


CPig * CPig::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos)
{
	CPig*		pInstance = new CPig(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
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
