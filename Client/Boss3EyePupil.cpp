#include "stdafx.h"
#include "Boss3EyePupil.h"
#include "Export_Function.h"

#include "Toodee.h"
#include "Topdee.h"

CBoss3EyePupil::CBoss3EyePupil(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev),
	m_fDamagedTime(0.f),
	m_bDamaged(false)
{
	m_pBoss3 = nullptr;
	m_pToodee = nullptr;
	m_pTopdee = nullptr;
}

CBoss3EyePupil::~CBoss3EyePupil()
{
}

HRESULT CBoss3EyePupil::Ready_GameObject(_vec3 & vPos, _int iIndex)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTransform->m_vInfo[INFO_POS] = _vec3{ vPos.x, vPos.y, vPos.z };
	m_pTransform->Rotation(ROT_Y, D3DXToRadian(45.f));
	m_pTransform->m_bIsStatic = true;

	m_iIndex = iIndex;

	m_pBoss3 = Engine::Get_GameObject(L"Layer_GameLogic", L"Boss3");
	NULL_CHECK_RETURN(m_pBoss3, E_FAIL);

	m_pToodee = dynamic_cast<CToodee*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Toodee"));
	m_pTopdee = dynamic_cast<CTopdee*>(Engine::Get_GameObject(L"Layer_GameLogic", L"Topdee"));

	return S_OK;
}

_int CBoss3EyePupil::Update_GameObject(const _float & fTimeDelta)
{
	if (m_bDead)
		return OBJ_DEAD;

	if (m_bDamaged)
		DamagedBoss3(fTimeDelta);

	__super::Update_GameObject(fTimeDelta);

	m_pTransform->Set_ParentTransform(m_pBoss3);

	if (m_pTopdee->m_bDead || m_pToodee->m_bDead)
		return 0;

	LookAtPlayer();

	Engine::Add_RenderGroup(RENDER_ALPHA, this);

	return 0;
}

void CBoss3EyePupil::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CBoss3EyePupil::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	if (m_bDamaged)
	{
		if (0 == (_int)(m_fDamagedTime * 250) % 2)
			m_pTextureCom->Set_Texture(m_iIndex);

		else
			m_pTextureCom->Set_Texture(5);
	}

	else
		m_pTextureCom->Set_Texture(m_iIndex);

	m_pBufferCom->Render_Buffer();

	__super::Render_GameObject();
}

HRESULT CBoss3EyePupil::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CRcTex*>(Engine::Clone_Proto(L"RcTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"RcTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Boss3_Eye", this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Boss3_Eye", pComponent });

	return S_OK;
}

void CBoss3EyePupil::LookAtPlayer()
{
	_vec3 vPos;
	CGameObject* pToodee = nullptr;
	CGameObject* pTopdee = nullptr;

	m_pTransform->m_vInfo[INFO_POS].y -= m_vDir.y;
	m_pTransform->m_vInfo[INFO_POS].z += m_vDir.x;

	if (g_Is2D)
	{
		pToodee = Engine::Get_GameObject(L"Layer_GameLogic", L"Toodee");
		NULL_CHECK_RETURN(pToodee, );
		vPos = pToodee->m_pTransform->m_vInfo[INFO_POS];
	}

	else
	{
		pTopdee = Engine::Get_GameObject(L"Layer_GameLogic", L"Topdee");
		if (pTopdee == nullptr)
			return;
		vPos = pTopdee->m_pTransform->m_vInfo[INFO_POS];
	}
	
	D3DXVec3Normalize(&m_vDir, &(vPos - m_pBoss3->m_pTransform->m_vInfo[INFO_POS]));

	m_pTransform->m_vInfo[INFO_POS].y += m_vDir.y;
	m_pTransform->m_vInfo[INFO_POS].z -= m_vDir.x;
}

void CBoss3EyePupil::DamagedBoss3(const _float & fTimeDelta)
{
	m_fDamagedTime += fTimeDelta * 0.01f;

	if (3.f <= m_fDamagedTime * 100.f)
	{
		m_bDamaged = false;
		m_fDamagedTime = 0.f;
	}
}

CBoss3EyePupil * CBoss3EyePupil::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos, _int iIndex)
{
	CBoss3EyePupil* pInstance = new CBoss3EyePupil(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, iIndex)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CBoss3EyePupil::Free(void)
{
	__super::Free();
}