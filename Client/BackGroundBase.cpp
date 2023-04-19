#include "stdafx.h"
#include "BackGroundBase.h"

CBackGroundBase::CBackGroundBase(LPDIRECT3DDEVICE9 pGraphicDev)
	:
	CGameObject(pGraphicDev),
	m_fTimer(0.0f),
	m_fSpeed(0.0f)
{
}

CBackGroundBase::~CBackGroundBase()
{
}

_int CBackGroundBase::Update_GameObject(const _float & fTimeDelta)
{
	__super::Update_GameObject(fTimeDelta);

	return 0;
}

_int CBackGroundBase::Update_Too(const _float & fTimeDelta)
{
	__super::Update_Too(fTimeDelta);

	return 0;
}

_int CBackGroundBase::Update_Top(const _float & fTimeDelta)
{
	__super::Update_Top(fTimeDelta);

	return 0;
}

void CBackGroundBase::Render_Too()
{
	__super::Render_Too();
}

void CBackGroundBase::Render_Top()
{
	__super::Render_Top();
}

void CBackGroundBase::Render_GameObject(void)
{
	__super::Render_GameObject();
}

CBackGroundBase * CBackGroundBase::Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 & vPos, _float fScale)
{
	CBackGroundBase*		pInstance = new CBackGroundBase(pGraphicDev);

	if (FAILED(pInstance->Ready_GameObject(vPos, fScale)))
	{
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

HRESULT CBackGroundBase::Ready_GameObject(_vec3 & vPos, _float fScale)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_pTransform->m_vInfo[INFO_POS] = vPos;

	m_pTransform->m_vScale = { fScale,fScale,1.0f };

	return S_OK;
}

HRESULT CBackGroundBase::Add_Component(void)
{
	return S_OK;
}

void CBackGroundBase::Free(void)
{
	__super::Free();
}
