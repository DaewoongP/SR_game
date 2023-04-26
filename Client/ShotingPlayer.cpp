#include "stdafx.h"
#include "ShotingPlayer.h"


CShotingPlayer::CShotingPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev)
{

}


CShotingPlayer::~CShotingPlayer()
{
}

HRESULT CShotingPlayer::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	__super::Ready_GameObject(vPos);




	return S_OK;
}

_int CShotingPlayer::Update_GameObject(const _float & fTimeDelta)
{
	return _int();
}

_int CShotingPlayer::Update_Too(const _float & fTimeDelta)
{
	return _int();
}

_int CShotingPlayer::Update_Top(const _float & fTimedDelte)
{
	return _int();
}

void CShotingPlayer::LateUpdate_GameObject(void)
{
}

void CShotingPlayer::Render_GameObject(void)
{
}

void CShotingPlayer::SwapTrigger()
{
}

void CShotingPlayer::OnCollisionEnter(const Collision * collision)
{
}

void CShotingPlayer::OnCollisionStay(const Collision * collision)
{
}

void CShotingPlayer::OnCollisionExit(const Collision * collision)
{
}

void CShotingPlayer::Set_Die()
{
}

void CShotingPlayer::Key_Input(const _float & fTimeDelta)
{

}

HRESULT CShotingPlayer::Add_Component(void)
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

CShotingPlayer * CShotingPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	return nullptr;
}

void CShotingPlayer::Free(void)
{
}
