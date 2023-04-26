#include "stdafx.h"
#include "ShootingPlayer.h"


CShootingPlayer::CShootingPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{

}


CShootingPlayer::~CShootingPlayer()
{
}

HRESULT CShootingPlayer::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	__super::Ready_GameObject(vPos);




	return S_OK;
}

_int CShootingPlayer::Update_GameObject(const _float & fTimeDelta)
{
	return _int();
}

_int CShootingPlayer::Update_Too(const _float & fTimeDelta)
{
	return _int();
}

_int CShootingPlayer::Update_Top(const _float & fTimedDelte)
{
	return _int();
}

void CShootingPlayer::LateUpdate_GameObject(void)
{
}

void CShootingPlayer::Render_GameObject(void)
{
}

void CShootingPlayer::SwapTrigger()
{
}

void CShootingPlayer::OnCollisionEnter(const Collision * collision)
{
}

void CShootingPlayer::OnCollisionStay(const Collision * collision)
{
}

void CShootingPlayer::OnCollisionExit(const Collision * collision)
{
}

void CShootingPlayer::Set_Die()
{
}

void CShootingPlayer::Key_Input(const _float & fTimeDelta)
{

}

HRESULT CShootingPlayer::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Yoga", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Yoga", pComponent });

	return S_OK;
}

CShootingPlayer * CShootingPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	return nullptr;
}

void CShootingPlayer::Free(void)
{
}
