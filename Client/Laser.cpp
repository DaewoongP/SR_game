#include "stdafx.h"
#include "Laser.h"
#include"ThirdCamera.h"

CLaser::CLaser(LPDIRECT3DDEVICE9 pGraphicDev)
	:CBullet(pGraphicDev)
{
}

CLaser::~CLaser()
{
}

HRESULT CLaser::Ready_Bullet(_vec3& vPos, _vec3& vDir)
{
	m_vPos = Get_GameObject(L"Layer_GameLogic", L"Topdee")->m_pTransform->m_vInfo[INFO_POS];
	m_vTarget= Get_GameObject(L"Layer_GameLogic", L"Final3Boss1")->m_pTransform->m_vInfo[INFO_POS];
	vPos = m_vPos;
	__super::Ready_Bullet(vPos);
	m_pTransform->m_vAngle.z = D3DXToRadian(90.f);
	m_pTransform->m_vAngle.x = D3DXToRadian(90.f);
	m_pTransform->m_vScale.y *= 3.f;
	m_pTransform->m_vScale.x=D3DXVec3Length(&(m_vPos-m_vTarget))/2.f;
	//m_pTransform->m_vInfo[INFO_POS].y = vPos.y;
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_pTex->Add_Anim(L"Idle", 0, 6, 0.3f, true);
	m_pTex->Switch_Anim(L"Idle");
	m_pTex->m_bUseFrameAnimation = true;
	m_bShoot = false;
	m_fEndTime = 0.f;
	return S_OK;
}

void CLaser::Ready_Pool(_vec3& vPos, _vec3& vDir)
{
	m_vPos = Get_GameObject(L"Layer_GameLogic", L"Topdee")->m_pTransform->m_vInfo[INFO_POS];
	m_vTarget = Get_GameObject(L"Layer_GameLogic", L"Final3Boss1")->m_pTransform->m_vInfo[INFO_POS];
	vPos = m_vPos;
	__super::Ready_Bullet(vPos);
	m_pTransform->m_vAngle.y = D3DXToRadian(-100.f);
	m_pTransform->m_vAngle.z = D3DXToRadian(90.f);
	m_pTransform->m_vAngle.x = D3DXToRadian(-10.f);

	m_pTransform->m_vScale.y *= 3.f;
	m_pTransform->m_vScale.x = D3DXVec3Length(&(m_vPos - m_vTarget)) / 2.f;
	//m_pTransform->m_vInfo[INFO_POS].y = vPos.y;
	m_pTex->Add_Anim(L"Idle", 0, 6, 0.3f, true);
	m_pTex->Switch_Anim(L"Idle");
	m_pTex->m_bUseFrameAnimation = true;
	m_bShoot = false;
	return ;
}

_int CLaser::Update_GameObject(const _float& fTimeDelta)
{
	m_fEndTime += fTimeDelta;
	if (Get_GameObject(L"Layer_GameLogic", L"Topdee") != nullptr)
		m_vPos = Get_GameObject(L"Layer_GameLogic", L"Topdee")->m_pTransform->m_vInfo[INFO_POS];
	if (Get_GameObject(L"Layer_GameLogic", L"Final3Boss1") != nullptr)
		m_vTarget = Get_GameObject(L"Layer_GameLogic", L"Final3Boss1")->m_pTransform->m_vInfo[INFO_POS];
	m_pTransform->m_vInfo[INFO_POS] = { m_vPos.x,m_vPos.y - 5.f, m_vPos.z + (D3DXVec3Length(&(m_vPos - m_vTarget)) / 2.f) - 10.f };
	m_pTex->Update_Anim(fTimeDelta);
	_vec3 vec = m_pTransform->m_vScale;

	m_pCollider->Set_BoundingBox({ vec.y,vec.x,30.f });
	if (m_fEndTime > 3.5f)
	{
		m_pTransform->m_vScale.y -= 1.f;
		if (m_pTransform->m_vScale.y <= 0)
			m_pTransform->m_vScale.y = 0.f;
	}

	__super::Update_GameObject(fTimeDelta);
	return 0;
}

void CLaser::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CLaser::Render_GameObject(void)
{
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransform->Get_WorldMatrixPointer());

	m_pTex->Set_Texture();
	m_pBuf->Render_Buffer();

	__super::Render_GameObject();
}

void CLaser::OnCollisionEnter(const Collision* collision)
{
	collision->otherObj->Set_Damage();
}

HRESULT CLaser::Add_Component(void)
{
	CComponent* pComponent = nullptr;
	pComponent = m_pTex = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"ShootingLaser", this));
	NULL_CHECK_RETURN(m_pTex, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"ShootingLaser", pComponent });
	
	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });
	return S_OK;
}

CLaser* CLaser::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3& vPos)
{
	CLaser* pInstance = new CLaser(pGraphicDev);
	if (FAILED(pInstance->Ready_Bullet(vPos)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CLaser::Free(void)
{
	__super::Free();
}
