#include "stdafx.h"
#include "GravityCube.h"
#include "Export_Function.h"
#include "CrackCube.h"

CGravityCube::CGravityCube(LPDIRECT3DDEVICE9 pGraphicDev):CMoveCube(pGraphicDev)
{
}

CGravityCube::~CGravityCube()
{
}

HRESULT CGravityCube::Ready_GameObject(_vec3 & vPos)
{
	__super::Ready_GameObject(vPos);
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_bUseGraivty = false;
	m_pTransform->m_bIsStatic = false;
	m_pCollider->m_bIsTrigger = false;
	m_pCollider->Set_BoundingBox({ 2.f,2.f,2.f });
	return S_OK;
}

_int CGravityCube::Update_GameObject(const _float & fTimeDelta)
{
	__super::Update_GameObject(0.02f);
	Engine::Add_RenderGroup(RENDER_NONALPHA, this);
	return 0;
}

_int CGravityCube::Update_Too(const _float & fTimeDelta)
{
	Do_CheckRay_Down();
	if (m_bUseGraivty)
	{
		m_pTransform->m_vInfo[INFO_POS].y -= 9.8*fTimeDelta;
	}
	__super::Update_Too(fTimeDelta);
	return 0;
}

_int CGravityCube::Update_Top(const _float & fTimeDelta)
{
	__super::Update_Top(fTimeDelta);
	return 0;
}

void CGravityCube::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CGravityCube::Render_GameObject(void)
{
	__super::Render_GameObject();
}

void CGravityCube::SwapTrigger()
{
	if (!g_Is2D)
	{
		m_pTransform->m_vInfo[INFO_POS].x = 
			((int)m_pTransform->m_vInfo[INFO_POS].x%2==0)? ((int)m_pTransform->m_vInfo[INFO_POS].x) : ((int)m_pTransform->m_vInfo[INFO_POS].x+1);
		m_pTransform->m_vInfo[INFO_POS].y = 
			((int)m_pTransform->m_vInfo[INFO_POS].y % 2 == 0) ? ((int)m_pTransform->m_vInfo[INFO_POS].y) : ((int)m_pTransform->m_vInfo[INFO_POS].y + 1);
		m_pTransform->m_vInfo[INFO_POS].z = 
			((int)m_pTransform->m_vInfo[INFO_POS].z % 2 == 0) ? ((int)m_pTransform->m_vInfo[INFO_POS].z) : ((int)m_pTransform->m_vInfo[INFO_POS].z + 1);
	}
}

void CGravityCube::OnCollisionEnter(const Collision * collision)
{
	__super::OnCollisionEnter(collision);
}

void CGravityCube::OnCollisionStay(const Collision * collision)
{
	__super::OnCollisionStay(collision);
}

void CGravityCube::OnCollisionExit(const Collision * collision)
{
	__super::OnCollisionExit(collision);
}

CGravityCube * CGravityCube::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos)
{
	CGravityCube*		pInstance = new CGravityCube(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CGravityCube::Do_CheckRay_Down()
{
	vector<RayCollision> _detectedCOL = Engine::Check_Collision_Ray(RAYCAST(m_pTransform->m_vInfo[INFO_POS], _vec3(0,-1,0), 1.001f), m_pCollider);
	if (_detectedCOL.size() >= 1)
	{
		if (dynamic_cast<CCube*>(_detectedCOL[0].col->m_pGameObject))
		{
			if (!lstrcmp(_detectedCOL[0].tag, L"CrackCube")&&
				!dynamic_cast<CCrackCube*>(_detectedCOL[0].col->m_pGameObject)->GetCrackDead())
			{
				dynamic_cast<CCrackCube*>(_detectedCOL[0].col->m_pGameObject)->DoShootRay(DIR_DOWN);
			}
			m_pTransform->m_bIsStatic = true;
			m_bUseGraivty = false;
		}
		else
		{
			m_pTransform->m_bIsStatic = false;
			m_bUseGraivty = true;
		}
	}
	else
	{
		m_pTransform->m_bIsStatic = false;
		m_bUseGraivty = true;
	}
}

void CGravityCube::Free(void)
{
	__super::Free();
}

HRESULT CGravityCube::Add_Component(void)
{
	return S_OK;
}
