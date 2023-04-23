#include "stdafx.h"
#include "Fireball.h"

#include"..\Engine\Export_Function.h"
CFireball::CFireball(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev),
	m_bSetTarget(true), m_bStaticON(true),
	m_fSpeed(8.f), m_fStaticOFF(0.f), m_fDegree(0.f)
{

}

CFireball::~CFireball()
{
}

HRESULT CFireball::Ready_GameObject(_vec3& vPos)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = vPos;

	m_pTransform->m_bIsStatic = true;

	m_pTextureCom->Add_Anim(L"Idle", 0, 8, 0.5f, true);
	m_pTextureCom->Switch_Anim(L"Idle");
	m_pTextureCom->m_bUseFrameAnimation = true;

	m_pCollider->Set_Options({ 2.f, 2.f, 2.f }, COL_ENV, false);
	StopSound(SOUND_EFFECT_ENEMY);
	PlaySound_Effect(L"1.wav", SOUND_EFFECT_ENEMY, 1.f);
	return S_OK;
}

_int CFireball::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_bSetTarget)
	{
		CGameObject* pGameObject = Engine::Get_GameObject(L"Layer_GameLogic", L"Toodee");
		if (nullptr == pGameObject)
			return 0;

		_vec3 vToodee = pGameObject->m_pTransform->m_vInfo[INFO_POS];

		D3DXVec3Normalize(&m_vDir, &(vToodee - m_pTransform->m_vInfo[INFO_POS]));

		_vec3 vRight = { -1.f, 0.f, 0.f };

		_float fAngle = acosf(D3DXVec3Dot(&m_vDir, &vRight));

		if (vToodee.y > m_pTransform->m_vInfo[INFO_POS].y)
			fAngle = 2 * 3.14f - fAngle;

		m_pTransform->Rotation(ROT_Z, fAngle);

		m_fDegree = D3DXToDegree(fAngle);

		m_bSetTarget = false;
	}

	if (m_bStaticON)
	{
		m_fStaticOFF += fTimeDelta;

		m_pTransform->m_vScale = {1.f + m_fStaticOFF,  1.f + m_fStaticOFF, 1.f + m_fStaticOFF, };

		if (1.f < m_fStaticOFF)
		{
			m_bStaticON = false;
			m_pTransform->m_vScale = {5.f, 2.5f, 1.f};
			m_pCollider->Set_BoundingBox({ 5.f, 2.5f, 1.f });
			m_pTransform->m_vInfo[INFO_POS].z = 10.f;

			if (0.f <= m_fDegree && 45.f > m_fDegree)
				m_pCollider->Set_BoundOffset(_vec3{ -2.f, -1.f, 0.f });

			else if (45.f <= m_fDegree && 90.f > m_fDegree)
				m_pCollider->Set_BoundOffset(_vec3{ -1.f, -2.f, 0.f });

			else if (90.f <= m_fDegree && 135.f > m_fDegree)
				m_pCollider->Set_BoundOffset(_vec3{ 1.f, -2.f, 0.f });

			else if (135.f <= m_fDegree && 180.f > m_fDegree)
				m_pCollider->Set_BoundOffset(_vec3{ 2.f, -1.f, 0.f });

			else if (180.f <= m_fDegree && 225.f > m_fDegree)
				m_pCollider->Set_BoundOffset(_vec3{ 2.f, 1.f, 0.f });

			else if (225.f <= m_fDegree && 270.f > m_fDegree)
				m_pCollider->Set_BoundOffset(_vec3{ 1.f, 2.f, 0.f });

			else if (270.f <= m_fDegree && 315.f > m_fDegree)
				m_pCollider->Set_BoundOffset(_vec3{ -1.f, 2.f, 0.f });

			else if (315.f <= m_fDegree && 360.f > m_fDegree)
				m_pCollider->Set_BoundOffset(_vec3{ -2.f, 1.f, 0.f });
		}			
	}

	m_pTransform->m_vInfo[INFO_POS].x += m_vDir.x * m_fSpeed * fTimeDelta;
	m_pTransform->m_vInfo[INFO_POS].y += m_vDir.y * m_fSpeed * fTimeDelta;

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	__super::Update_GameObject(fTimeDelta);

	m_pTextureCom->Update_Anim(fTimeDelta);

	return 0;
}

_int CFireball::Update_Too(const _float& fTimeDelta)
{
	return S_OK;
}

_int CFireball::Update_Top(const _float& fTimeDelta)
{
	return S_OK;
}

void CFireball::LateUpdate_GameObject(void)
{
	if (0.f > m_pTransform->m_vInfo[INFO_POS].x ||
		64.f < m_pTransform->m_vInfo[INFO_POS].x ||
		0.f > m_pTransform->m_vInfo[INFO_POS].y ||
		36.f < m_pTransform->m_vInfo[INFO_POS].y)
		m_bDead = true;

	__super::LateUpdate_GameObject();
}

void CFireball::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	if (m_bStaticON)
		m_pCreateTextureCom->Set_Texture();

	else
		m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();
	__super::Render_GameObject();
}

void CFireball::OnCollisionEnter(const Collision* collision)
{
	if (!lstrcmp(collision->otherObj->m_pTag, L"LaserTurret"))
		m_bDead = true;

	if (!lstrcmp(collision->otherObj->m_pTag, L"Toodee")&& (g_Is2D == true))
		m_bDead = true;
}

HRESULT CFireball::Add_Component(void)
{
	CComponent* pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex" , pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Fireball_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Fireball_Texture",pComponent });

	pComponent = m_pCreateTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Fireball_Create_Texture", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Fireball_Create_Texture",pComponent });

	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider",pComponent });

	return S_OK;
}

CFireball* CFireball::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos)
{
	CFireball* pInstance = new CFireball(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CFireball::Free(void)
{
	__super::Free();
}