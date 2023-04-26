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

	return S_OK;
}

_int CShootingPlayer::Update_GameObject(const _float & fTimeDelta)
{
	return OBJ_NOEVENT;
}

_int CShootingPlayer::Update_Too(const _float & fTimeDelta)
{
	return 0;
}

_int CShootingPlayer::Update_Top(const _float & fTimedDelte)
{
	return 0;
}

void CShootingPlayer::LateUpdate_GameObject(void)
{
}

void CShootingPlayer::Render_GameObject(void)
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

void CShootingPlayer::Key_Input(const _float & fTimeDelta)
{
	if (Engine::Get_DIKeyState(DIK_LEFT) == Engine::KEYDOWN)
	{
	}
	if (Engine::Get_DIKeyState(DIK_RIGHT) == Engine::KEYDOWN)
	{
	}
}

HRESULT CShootingPlayer::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"ThirddeeShooting", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"ThirddeeShooting", pComponent });

	return S_OK;
}

CShootingPlayer * CShootingPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CShootingPlayer* pInstance = new CShootingPlayer(pGraphicDev);
	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CShootingPlayer::Free(void)
{
}
