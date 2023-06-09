#include "stdafx.h"
#include "ShootingPlayer.h"
#include "..\Engine\AbstractFactory.h"
#include "Export_Function.h"
#include"ShootingCamera.h"
#include"Laser.h"

CShootingPlayer::CShootingPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev), m_pGameLogicLayer(nullptr),
	m_iBulletIndex(0)
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
	m_pCollider->Set_Options();
	m_vPos[INIT] = vPos;
	m_vPos[LEFT] = vPos + _vec3(-6.5f, 5.f, 0.f);
	m_vPos[RIGHT] = vPos + _vec3(6.5f, 5.f, 0.f);
	m_vPrePos = vPos;
	m_OriginPos = _vec3(0, 0, 0);
	m_OriginRot = _vec3(0, 0, 0);
	m_bPossibleShoot = true;
	m_bLKey = true;
	m_bRKey = true;
	m_pTransform->m_vAngle.x = D3DXToRadian(+30);
	m_fSlerp = 0.f;
	Engine::Ready_Frame(L"2Sec", 0.5f);
	
	return S_OK;
}

_int CShootingPlayer::Update_GameObject(const _float & fTimeDelta)
{
	if (!m_bUseUpdate)
		return 0;
	CGameObject* pLaser = nullptr;

	Add_RenderGroup(RENDER_ALPHA, this);

	if (m_bPossibleShoot)
	{
		Key_Input(fTimeDelta);
		Rot_Player();
	}
	if (m_pGameLogicLayer == nullptr)
		m_pGameLogicLayer = Engine::Get_Layer(L"Layer_GameLogic");
	if (m_iBulletIndex == 4)
		pLaser = Get_GameObject(L"Layer_GameLogic", L"ShootingLaser");

	if (pLaser != nullptr)
	{
		m_fLaserTime += fTimeDelta;
		if (m_fLaserTime > 2.5f && pLaser->m_pTransform->m_vScale.y < 30.f)
		{
			pLaser->m_pTransform->m_vScale.y += 0.3f;
			pLaser->m_pTransform->m_vInfo[INFO_POS].z -= 1.f;
			pLaser->Set_D_T();
		}
	}

	if (m_bPossibleShoot)
		Shoot_Bullet(fTimeDelta);
	__super::Update_GameObject(fTimeDelta);

	return OBJ_NOEVENT;
}

void CShootingPlayer::LateUpdate_GameObject(void)
{
	if (!m_bUseUpdate)
		return;
	__super::LateUpdate_GameObject();
}

void CShootingPlayer::Render_GameObject(void)
{
	if (!m_bUseRender)
		return;
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTex->Set_Texture(0);
	m_pBuf->Render_Buffer();

	__super::Render_GameObject();
}

void CShootingPlayer::Key_Input(const _float & fTimeDelta)
{
	_vec3 vUp = { 0,1,0 };

	if (Engine::Get_DIKeyState(DIK_F) == Engine::KEYPRESS)
	{
		m_iBulletIndex = 4;
	}
	if (Engine::Get_DIKeyState(DIK_LEFT) == Engine::KEYPRESS && m_bLKey)
	{
		m_bRKey = false;
		m_fSlerp += fTimeDelta;
		if (m_fSlerp >= 1.f)
			m_fSlerp = 1.f;
		vUp = { 0, 0, -1 };
		GetVectorSlerp(&m_pTransform->m_vInfo[INFO_POS], &m_vPos[INIT], &m_vPos[LEFT], &vUp, 5.f, m_fSlerp);

		m_vPrePos = m_pTransform->m_vInfo[INFO_POS];
		return;
	}
	else if (Engine::Get_DIKeyState(DIK_RIGHT) == Engine::KEYPRESS && m_bRKey)
	{
		m_bLKey = false;
		m_fSlerp += fTimeDelta;
		if (m_fSlerp >= 1.f)
			m_fSlerp = 1.f;
		vUp = { 0, 0, +1 };
		GetVectorSlerp(&m_pTransform->m_vInfo[INFO_POS], &m_vPos[INIT], &m_vPos[RIGHT], &vUp, 5.f, m_fSlerp);

		m_vPrePos = m_pTransform->m_vInfo[INFO_POS];
		return;
	}
	else if (Engine::Get_DIKeyState(DIK_LEFT) == Engine::KEYUP ||
		Engine::Get_DIKeyState(DIK_RIGHT) == Engine::KEYUP)
	{
		m_fSlerp = 1.f;
		m_bLKey = false;
		m_bRKey = false;
	}
	m_fSlerp -= fTimeDelta;
	if (m_fSlerp <= 0.f)
	{
		m_fSlerp = 0.f;
		m_bLKey = true;
		m_bRKey = true;
		m_pTransform->m_vAngle.y = 0.f;
		return;
	}
	
	// s Lerp Ã³¸®°¡ ¾È¸ÔÀ½.
	D3DXVec3Lerp(&m_pTransform->m_vInfo[INFO_POS], &m_vPrePos, &m_vPos[INIT], 1 - m_fSlerp);
}

void CShootingPlayer::Shoot_Bullet(const _float & fTimeDelta)
{
	switch (m_iBulletIndex)
	{
	case 0:
		Default_Bullet(fTimeDelta);
		break;
	case 1:
		Quad_Bullet(fTimeDelta);
		break;
	case 2:
		Sword_Bullet(fTimeDelta);
		break;
	case 3:
		Fire_Bullet(fTimeDelta);
		break;
	case 4:
		Laser(fTimeDelta);
		break;
	default:
		Default_Bullet(fTimeDelta);
		break;
	}
}

void CShootingPlayer::Default_Bullet(const _float& fTimeDelta)
{
	if (IsPermit_Call(L"0.5Sec", fTimeDelta))
	{
		CBullet* pBullet = nullptr;
		pBullet = Engine::Reuse_Bullet(m_pGraphicDev, m_pTransform->m_vInfo[INFO_POS], DEFAULT);
		if (pBullet == nullptr)
			return;
		m_pGameLogicLayer->Add_GameObject(L"Bullet", pBullet);
	}
}

void CShootingPlayer::Quad_Bullet(const _float & fTimeDelta)
{
	if (IsPermit_Call(L"0.5Sec", fTimeDelta))
	{
		CBullet* pBullet = nullptr;

		_vec3 vPos = m_pTransform->m_vInfo[INFO_POS];

		pBullet = Engine::Reuse_Bullet(m_pGraphicDev, _vec3{ vPos.x + 3.f, vPos.y, vPos.z }, DEFAULT);
		if (pBullet == nullptr) return;
		m_pGameLogicLayer->Add_GameObject(L"Bullet", pBullet);

		pBullet = Engine::Reuse_Bullet(m_pGraphicDev, _vec3{ vPos.x - 3.f, vPos.y, vPos.z }, DEFAULT);
		if (pBullet == nullptr) return;
		m_pGameLogicLayer->Add_GameObject(L"Bullet", pBullet);

		pBullet = Engine::Reuse_Bullet(m_pGraphicDev, _vec3{ vPos.x, vPos.y + 1.5f, vPos.z }, DEFAULT);
		if (pBullet == nullptr) return;
		m_pGameLogicLayer->Add_GameObject(L"Bullet", pBullet);

		pBullet = Engine::Reuse_Bullet(m_pGraphicDev, _vec3{ vPos.x, vPos.y - 1.5f, vPos.z }, DEFAULT);
		if (pBullet == nullptr)	return;
		m_pGameLogicLayer->Add_GameObject(L"Bullet", pBullet);
	}
}

void CShootingPlayer::Sword_Bullet(const _float & fTimeDelta)
{
	if (IsPermit_Call(L"1Sec", fTimeDelta))
	{
		CBullet* pBullet = nullptr;
		pBullet = Engine::Reuse_Bullet(m_pGraphicDev, m_pTransform->m_vInfo[INFO_POS], SWORD);
		if (pBullet == nullptr) return;
		m_pGameLogicLayer->Add_GameObject(L"SwordBullet", pBullet);
	}
}

void CShootingPlayer::Fire_Bullet(const _float& fTimeDelta)
{
	for (size_t i = 0; i < 20; ++i)
	{
		if (IsPermit_Call(L"2Sec", fTimeDelta))
		{
			CBullet* pBullet = nullptr;
			pBullet = Engine::Reuse_Bullet(m_pGraphicDev, m_pTransform->m_vInfo[INFO_POS], FIRE);
			if (pBullet == nullptr) return;
			m_pGameLogicLayer->Add_GameObject(L"FireBullet", pBullet);
		}
	}
}
void CShootingPlayer::Laser(const _float& fTimeDelta)
{
	if (m_bLaser)
	{
		CLaser* pBullet = nullptr;

		pBullet = CLaser::Create(m_pGraphicDev, m_pTransform->m_vInfo[INFO_POS]);
		if (pBullet == nullptr)
			return;
		m_pGameLogicLayer->Add_GameObject(L"ShootingLaser", pBullet);
		m_bLaser = false;
	}
}
void CShootingPlayer::Rot_Player()
{
	_vec3 vMin = m_pTransform->m_vInfo[INFO_POS] - m_vPos[INIT];
	_float fLen = D3DXVec3Length(&vMin);

	if (0 < vMin.x)
	{
		m_pTransform->m_vAngle.z = D3DXToRadian(fLen * 3.f);
		m_pTransform->m_vAngle.y = D3DXToRadian(fLen * 4.f);
	}
	else
	{
		m_pTransform->m_vAngle.z = D3DXToRadian(-fLen * 3.f);
		m_pTransform->m_vAngle.y = D3DXToRadian(-fLen * 4.f);
	}
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

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });

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
	__super::Free();
}
