#include "stdafx.h"
#include "Toodee.h"

#include "Export_Function.h"

CToodee::CToodee(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_bJumpable(false)
	, m_eKeyState(DIR_END)
{
}

CToodee::~CToodee()
{
}

HRESULT CToodee::Ready_GameObject(_vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_bIsStatic = false;
	// 애니메이션
	m_pTextureCom->Add_Anim(L"Idle", 0, 5, 1.f, true);
	m_pTextureCom->Add_Anim(L"Walk", 6, 13, 1.f, true);
	m_pTextureCom->Add_Anim(L"Jump", 26, 30, 1.f, false);
	m_pTextureCom->Add_Anim(L"Die", 67, 72, 0.6f, false);
	m_pTextureCom->Switch_Anim(L"Idle");
	m_pTextureCom->m_bUseFrameAnimation = true;

	m_pCollider->Set_BoundingBox({ 1.f,2.f,1.0f });

	return S_OK;
}
_int CToodee::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_ALPHA, this);
	
	return 0;
}
_int CToodee::Update_Too(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	Key_Input(fTimeDelta);
	DoStrech();
	__super::Update_GameObject(fTimeDelta);

	m_pTextureCom->Update_Anim(fTimeDelta);

	//텍스쳐컴의 애니가 die고 완료됐다면?
	if (m_pTextureCom->IsAnimationEnd(L"Die"))
		m_bDead = true;

	DoFlip();
	return 0;
}
void CToodee::SwapTrigger()
{
	if (g_Is2D)
	{
		m_pRigid->m_bUseGrivaty = true;
	}
	else 
	{
		m_pRigid->m_bUseGrivaty = false;
	}
}

void CToodee::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}
void CToodee::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());
	m_pTextureCom->Set_Texture(0);
	m_pBufferCom->Render_Buffer();
	__super::Render_GameObject();

}

void CToodee::OnCollisionEnter(const Collision * collision)
{
	//스파이크랑 충돌시 본인 데드 트루
	if (!lstrcmp(collision->otherObj->m_pTag, L"Spike") &&
		collision->_dir == DIR_DOWN)
	{
		m_pTextureCom->Switch_Anim(L"Die");
	}

	__super::OnCollisionEnter(collision);
}

void CToodee::OnCollisionStay(const Collision * collision)
{
 	if (lstrcmp(m_pTextureCom->Get_AnimState(), L"Die"))
	{
		if (collision->_dir == DIR_DOWN)
			m_bJumpable = true;

		if (fabsf(m_pRigid->m_Velocity.y) > 2.f && m_bJumpable)
		{
			m_bJumpable = false;
			m_pTextureCom->Switch_Anim(L"Jump");
		}
		else if (m_bJumpable)
			if ((fabsf(m_pRigid->m_Velocity.x) > 1.f))
				m_pTextureCom->Switch_Anim(L"Walk");
			else
 				m_pTextureCom->Switch_Anim(L"Idle");
	}
	
	__super::OnCollisionStay(collision);
}

void CToodee::OnCollisionExit(const Collision * collision)
{
	m_bJumpable = false;
	__super::OnCollisionExit(collision);
}

HRESULT CToodee::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Player_Texture", this));
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


CToodee* CToodee::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos)
{
	CToodee*		pInstance = new CToodee(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CToodee::Free(void)
{
	__super::Free();
}

void CToodee::Key_Input(const _float & fTimeDelta)
{
	if (Engine::Get_DIKeyState(DIK_LEFT) == Engine::KEYDOWN)
		m_eKeyState = DIR_LEFT;

	if (Engine::Get_DIKeyState(DIK_RIGHT) == Engine::KEYDOWN)
		m_eKeyState = DIR_RIGHT;

	if (Engine::Get_DIKeyState(DIK_LEFT) == Engine::KEYPRESS)
		m_pRigid->m_Velocity.x = -m_fSpeed;
	if (Engine::Get_DIKeyState(DIK_RIGHT) == Engine::KEYPRESS)
		m_pRigid->m_Velocity.x = m_fSpeed;

	if (Engine::Get_DIKeyState(DIK_LEFT) == Engine::KEYUP)
		m_pRigid->m_Velocity.x = -m_fSpeed*0.2f;

	if (Engine::Get_DIKeyState(DIK_RIGHT) == Engine::KEYUP)
		m_pRigid->m_Velocity.x = m_fSpeed*0.2f;

	if (Engine::Get_DIKeyState(DIK_SPACE) == Engine::KEYDOWN && m_bJumpable)
		m_pRigid->AddForce(_vec3(0, 1, 0), 90.f, IMPULSE, fTimeDelta);

}

void CToodee::DoFlip()
{
	if (m_eKeyState == DIR_LEFT)
		m_pTransform->m_vAngle.y = Lerp(m_pTransform->m_vAngle.y, D3DXToRadian(180), 0.1f);
	else if (m_eKeyState == DIR_RIGHT)
		m_pTransform->m_vAngle.y = Lerp(m_pTransform->m_vAngle.y, 0, 0.1f);

}

void CToodee::DoStrech()
{
	if (-1.f > m_pRigid->m_Velocity.y)
	{
		if (m_pTransform->m_vScale.y > 2.0f)
			return;
		m_pTransform->m_vScale.y *= 1.02f;
	}
	else
		m_pTransform->m_vScale.y = Lerp(m_pTransform->m_vScale.y, 1.f, 0.5f);
}
