#include "stdafx.h"
#include "Player.h"

#include "Export_Function.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_bJumpalbe(false)
	, m_eKeyState(DIR_END)
{

}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_GameObject(_vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	return S_OK;
}
_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}
_int CPlayer::Update_Too(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	m_pRigid->m_bUseGrivaty = true;
	Key_Input(fTimeDelta);
	DoStrech();
	__super::Update_GameObject(fTimeDelta);
	
	m_pTextureCom->Update_Anim(fTimeDelta);
	
	DoFlip();
	return 0;
}
_int CPlayer::Update_Top(const _float & fTimeDelta)
{
	m_pRigid->m_bUseGrivaty = false;

	return 0;
}

void CPlayer::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CPlayer::LateUpdate_Too()
{
}

void CPlayer::LateUpdate_Top()
{
}

void CPlayer::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
	
	__super::Render_GameObject();

}

void CPlayer::Render_Too()
{
}

void CPlayer::Render_Top()
{
}

void CPlayer::OnCollisionEnter(const Collision * collision)
{
	__super::OnCollisionEnter(collision);
}

void CPlayer::OnCollisionStay(const Collision * collision)
{
	if (collision->_dir == DIR_DOWN)
		m_bJumpalbe = true;

	if (fabsf(m_pRigid->m_Velocity.y) > 2.f && m_bJumpalbe)
	{
		m_bJumpalbe = false;
		m_pTextureCom->Switch_Anim(L"Jump");
	}
	else if (m_bJumpalbe)
		if ((fabsf(m_pRigid->m_Velocity.x)>1.f))
			m_pTextureCom->Switch_Anim(L"Walk");
		else
			m_pTextureCom->Switch_Anim(L"Idle");
	__super::OnCollisionStay(collision);
}

void CPlayer::OnCollisionExit(const Collision * collision)
{
	m_bJumpalbe = false;
	__super::OnCollisionExit(collision);
}

HRESULT CPlayer::Add_Component(void)
{
	CComponent*		pComponent = nullptr;
	m_pTransform->m_bIsStatic = false;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Player_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_uMapComponent[ID_STATIC].insert({ L"Texture", pComponent });
	m_pTextureCom->Add_Anim(L"Idle", 0, 5, 1.f, true);
	m_pTextureCom->Add_Anim(L"Walk", 6, 13, 1.f, true);
	m_pTextureCom->Add_Anim(L"Jump", 26, 30, 1.f, false);
	m_pTextureCom->Switch_Anim(L"Idle");
	m_pTextureCom->m_bUseFrameAnimation = true;

	pComponent = m_pRigid = dynamic_cast<CRigidbody*>(Engine::Clone_Proto(L"Rigidbody", this));
	NULL_CHECK_RETURN(m_pRigid, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Rigidbody", pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_uMapComponent[ID_DYNAMIC].insert({ L"Collider", pComponent });
	m_pCollider->Set_BoundingBox({ 1.f,2.f,0.2f });

	return S_OK;
}


CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos)
{
	CPlayer*		pInstance = new CPlayer(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CPlayer::Free(void)
{
	__super::Free();
}

void CPlayer::Key_Input(const _float & fTimeDelta)
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

	if (Engine::Get_DIKeyState(DIK_SPACE) == Engine::KEYDOWN && m_bJumpalbe)
    		m_pRigid->AddForce(_vec3(0, 1, 0), 90.f, IMPULSE, fTimeDelta);

}

void CPlayer::DoFlip()
{
	if (m_eKeyState == DIR_LEFT)
		m_pTransform->m_vAngle.y = Lerp(m_pTransform->m_vAngle.y, D3DXToRadian(180), 0.1f);
	else if (m_eKeyState == DIR_RIGHT)
		m_pTransform->m_vAngle.y = Lerp(m_pTransform->m_vAngle.y, 0, 0.1f);

}

void CPlayer::DoStrech()
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
