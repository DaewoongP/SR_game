#include "stdafx.h"
#include "FinalMonkeyCube.h"
#include "Export_Function.h"
#include "GravityCube.h"
CFinalMonkeyCube::CFinalMonkeyCube(LPDIRECT3DDEVICE9 pGraphicDev)
	:CCube(pGraphicDev)
{
}

CFinalMonkeyCube::~CFinalMonkeyCube()
{
}

HRESULT CFinalMonkeyCube::Ready_GameObject(_vec3 & vPos, vector<CGameObject*>& vec)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	m_isMoveDone = false;
	m_pTransform->m_vInfo[INFO_POS] = vPos;
	m_pTransform->m_vScale = { 1.f, 1.f, 1.f };

	_float ShakeValue = GetRandomFloat(5,10);
	m_Reverse = false;
	//Set_EarthQuake(ShakeValue*2, ShakeValue*30,SHAKE_Z);
	m_SleepTime = (10 - ShakeValue)/1;
	m_pTransform->m_vInfo[INFO_POS].z = GetRandomFloat(500,1000);
	m_fLerpPos_z = 10.f;
	m_pCollider->Set_BoundingBox({ 2.f,2.f,2.f });
	m_pCollider->Set_Group(COL_ENV);

	vec.push_back(this);
	return S_OK;
}

_int CFinalMonkeyCube::Update_GameObject(const _float & fTimeDelta)
{
	m_SleepTime -= fTimeDelta;
	if (m_SleepTime > 0)
		return __super::Update_GameObject(fTimeDelta);;
	if(!m_Reverse)
		m_pTransform->m_vInfo[INFO_POS].z = Lerp(m_pTransform->m_vInfo[INFO_POS].z, m_fLerpPos_z, fTimeDelta);
	else 
		m_pTransform->m_vInfo[INFO_POS].z = Lerp(m_fLerpPos_z, m_pTransform->m_vInfo[INFO_POS].z, 1-fTimeDelta);
	if (!m_isMoveDone&&fabsf(m_pTransform->m_vInfo[INFO_POS].z - m_fLerpPos_z) < 1)
		m_isMoveDone = true;
	return __super::Update_GameObject(fTimeDelta);
}

void CFinalMonkeyCube::LateUpdate_GameObject(void)
{
	__super::LateUpdate_GameObject();
}

void CFinalMonkeyCube::Render_GameObject(void)
{
	__super::Render_GameObject();
}

void CFinalMonkeyCube::OnCollisionEnter(const Collision * collision)
{
	__super::OnCollisionEnter(collision);
}

void CFinalMonkeyCube::OnCollisionStay(const Collision * collision)
{
	__super::OnCollisionStay(collision);
}

void CFinalMonkeyCube::OnCollisionExit(const Collision * collision)
{
	__super::OnCollisionExit(collision);
}

HRESULT CFinalMonkeyCube::Add_Component(void)
{
	CComponent*		pComponent = nullptr;

	pComponent = m_pBufferCom = dynamic_cast<CCubeTex*>(Engine::Clone_Proto(L"CubeTex", this));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"CubeTex", pComponent });

	pComponent = m_pTextureCom = dynamic_cast<CTexture*>(Engine::Clone_Proto(L"Stage_Cube",this));
	NULL_CHECK_RETURN(m_pTextureCom, E_FAIL);
	m_vecComponent[ID_STATIC].push_back({ L"Stage_Cube", pComponent });
  
	pComponent = m_pCollider = dynamic_cast<CCollider*>(Engine::Clone_Proto(L"Collider", this));
	NULL_CHECK_RETURN(m_pCollider, E_FAIL);
	m_vecComponent[ID_DYNAMIC].push_back({ L"Collider", pComponent });

	return S_OK;
}

CFinalMonkeyCube * CFinalMonkeyCube::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos, vector<CGameObject*>& vec, _int iIndex)
{
	CFinalMonkeyCube*		pInstance = new CFinalMonkeyCube(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, vec)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}
	pInstance->Set_CubeTexNum(iIndex);
	return pInstance;
}

void CFinalMonkeyCube::Free(void)
{
	__super::Free();
}
