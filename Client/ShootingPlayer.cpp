#include "stdafx.h"
#include "ShootingPlayer.h"
#include "..\Engine\AbstractFactory.h"
#include "Bullet.h"


CShootingPlayer::CShootingPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev), m_pGameLogicLayer(nullptr)
{
}


CShootingPlayer::~CShootingPlayer()
{
}

HRESULT CShootingPlayer::Ready_GameObject(_vec3 & vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vScale *= 2.f;
	m_pTransform->m_vInfo[INFO_POS] = vPos;

	m_vPos[INIT] = vPos;
	m_vPos[LEFT] = vPos + _vec3(-30.f, 0.f, -10.f);
	m_vPos[RIGHT] = vPos + _vec3(30.f, 10.f, -10.f);

	m_pTransform->m_vAngle.x = D3DXToRadian(-30);
	m_fSlerp = 0.f;
	return S_OK;
}

_int CShootingPlayer::Update_GameObject(const _float & fTimeDelta)
{
	Add_RenderGroup(RENDER_ALPHA, this);
	Key_Input(fTimeDelta);
	if (m_pGameLogicLayer == nullptr)
		m_pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	if (IsPermit_Call(L"1Sec", fTimeDelta))
		Fire_bullet();

	__super::Update_GameObject(fTimeDelta);
	return OBJ_NOEVENT;
}

void CShootingPlayer::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CShootingPlayer::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTex->Set_Texture(0);
	m_pBuf->Render_Buffer();

	__super::Render_GameObject();
}

void CShootingPlayer::Key_Input(const _float & fTimeDelta)
{
	_vec3 vUp = { 0,1,0 };
	if (Engine::Get_DIKeyState(DIK_LEFT) == Engine::KEYPRESS)
	{
		m_fSlerp += fTimeDelta;
		if (m_fSlerp >= 1.f)
			m_fSlerp = 1.f;
		GetVectorSlerp(&m_pTransform->m_vInfo[INFO_POS], &m_vPos[INIT], &m_vPos[LEFT], &vUp, 10.f, m_fSlerp);

		return;
	}
	if (Engine::Get_DIKeyState(DIK_RIGHT) == Engine::KEYPRESS)
	{
		m_fSlerp += fTimeDelta;
		if (m_fSlerp >= 1.f)
			m_fSlerp = 1.f;
		GetVectorSlerp(&m_pTransform->m_vInfo[INFO_POS], &m_vPos[INIT], &m_vPos[RIGHT], &vUp, 10.f, m_fSlerp);

		return;
	}
	m_fSlerp -= fTimeDelta;
	if (m_fSlerp <= 0.f)
	{
		m_fSlerp = 0.f;
		return;
	}
	GetVectorSlerp(&m_pTransform->m_vInfo[INFO_POS], &m_pTransform->m_vInfo[INFO_POS], &m_vPos[INIT], &vUp, 10.f, m_fSlerp);
}

HRESULT CShootingPlayer::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBuf = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBuf, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTex = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"ThirddeeShooting", this));
	NULL_CHECK_RETURN(m_pTex, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"ThirddeeShooting", pComponent });

	return S_OK;
}

void CShootingPlayer::Fire_bullet()
{
	FACTORY<CBullet>::Create(L"Bullet", m_pGameLogicLayer, m_pTransform->m_vInfo[INFO_POS]);
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
	__super::Free();
}
